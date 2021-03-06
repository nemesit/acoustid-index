// Copyright (C) 2011  Lukas Lalinsky
// Distributed under the MIT license, see the LICENSE file for details.

#ifndef ACOUSTID_INDEX_SEGMENT_MERGER_H_
#define ACOUSTID_INDEX_SEGMENT_MERGER_H_

#include "common.h"
#include "segment_enum.h"
#include "segment_data_writer.h"

namespace Acoustid {

class SegmentMerger
{
public:
	SegmentMerger(SegmentDataWriter *target);

	void addSource(SegmentEnum *reader)
	{
		m_readers.append(reader);
	}

	void merge();

private:
	QList<SegmentEnum *> m_readers;
	SegmentDataWriter *m_writer;
};

}

#endif
