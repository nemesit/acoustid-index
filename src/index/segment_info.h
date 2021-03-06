// Copyright (C) 2011  Lukas Lalinsky
// Distributed under the MIT license, see the LICENSE file for details.

#ifndef ACOUSTID_SEGMENT_INFO_H_
#define ACOUSTID_SEGMENT_INFO_H_

#include "common.h"

namespace Acoustid {

class SegmentInfo
{
public:
	SegmentInfo(const QString &name = QString(), size_t numDocs = 0)
		: m_name(name), m_numDocs(numDocs)
	{
	}

	QString name() const
	{
		return m_name;
	}

	void setName(const QString &name)
	{
		m_name = name;
	}

	size_t numDocs() const
	{
		return m_numDocs;
	}

	void setNumDocs(size_t numDocs)
	{
		m_numDocs = numDocs;
	}

private:
	QString m_name;
	size_t m_blockSize;
	size_t m_numDocs;
};

}

#endif
