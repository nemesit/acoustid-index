// Copyright (C) 2011  Lukas Lalinsky
// Distributed under the MIT license, see the LICENSE file for details.

#include <algorithm>
#include "store/directory.h"
#include "store/input_stream.h"
#include "store/output_stream.h"
#include "segment_index_writer.h"
#include "segment_data_writer.h"
#include "index_writer.h"

using namespace Acoustid;

IndexWriter::IndexWriter(Directory *dir, bool create)
	: m_dir(dir), m_numDocsInBuffer(0), m_maxSegmentBufferSize(1024 * 1024 * 10)
{
	m_revision = SegmentInfoList::findCurrentRevision(dir);
	if (m_revision != -1) {
		m_segmentInfos.read(dir->openFile(SegmentInfoList::segmentsFileName(m_revision)));
	}
	else if (create) {
		commit();
	}
	else {
		throw IOException("there is no index in the directory");
	}
}

IndexWriter::~IndexWriter()
{
}

int IndexWriter::revision()
{
	return m_revision;
}

void IndexWriter::addDocument(uint32_t id, uint32_t *terms, size_t length)
{
	for (size_t i = 0; i < length; i++) {
		m_segmentBuffer.push_back((uint64_t(terms[i]) << 32) | id);
	}
	m_numDocsInBuffer++;
	maybeFlush();
}

void IndexWriter::commit()
{
	flush();
	m_revision++;
	ScopedPtr<OutputStream> segmentsFile(m_dir->createFile(SegmentInfoList::segmentsFileName(m_revision)));
	m_segmentInfos.write(segmentsFile.get());
}

void IndexWriter::maybeFlush()
{
	if (m_segmentBuffer.size() > m_maxSegmentBufferSize) {
		flush();
	}
}

void IndexWriter::flush()
{
	if (m_segmentBuffer.empty()) {
		return;
	}
	std::sort(m_segmentBuffer.begin(), m_segmentBuffer.end());

	size_t num = m_segmentInfos.incNextSegmentNum();
	QString name = QString("segment_%1").arg(num);
	ScopedPtr<OutputStream> indexOutput(m_dir->createFile(name + ".fii"));
	ScopedPtr<OutputStream> dataOutput(m_dir->createFile(name + ".fid"));

	SegmentIndexWriter indexWriter(indexOutput.get());
	indexWriter.setBlockSize(512);

	SegmentDataWriter writer(dataOutput.get(), &indexWriter, indexWriter.blockSize());
	for (size_t i = 0; i < m_segmentBuffer.size(); i++) {
		uint32_t key = (m_segmentBuffer[i] >> 32);
		uint32_t value = m_segmentBuffer[i] & 0xffffffff;
		writer.addItem(key, value);
	}
	writer.close();

	m_segmentInfos.add(SegmentInfo(name, m_numDocsInBuffer));

	m_segmentBuffer.clear();
	m_numDocsInBuffer = 0;
}

