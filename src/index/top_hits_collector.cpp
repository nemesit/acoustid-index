// Acoustid Index -- Inverted index for audio fingerprints
// Copyright (C) 2011  Lukas Lalinsky
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include "top_hits_collector.h"

TopHitsCollector::TopHitsCollector(size_t numHits)
	: m_numHits(numHits)
{
}

TopHitsCollector::~TopHitsCollector()
{
}

void TopHitsCollector::collect(uint32_t id)
{
	m_counts[id] = m_counts[id] + 1;
}

struct CompareByCount
{
	CompareByCount(const QHash<uint32_t, unsigned int> &counts) : m_counts(counts) {}
	bool operator()(uint32_t a, uint32_t b)
	{
		return m_counts[a] > m_counts[b];
	}
	const QHash<uint32_t, unsigned int> &m_counts;
};

QList<Result> TopHitsCollector::topResults()
{
	QList<uint32_t> ids = m_counts.keys();
	std::sort(ids.begin(), ids.end(), CompareByCount(m_counts));
	QList<Result> results;
	for (int i = 0; i < std::min(m_numHits, size_t(ids.size())); i++) {
		uint32_t id = ids.at(i);
		results.append(Result(id, m_counts[id]));
	}
	return results;
}
