#include "NavigationGrid.h"
#include <queue>
#include <utility>

typedef std::pair<int, int>			CellMove;
typedef std::pair<float, CellMove>	Move;
typedef std::pair<float, Move>		ANode;


NavigationGrid::NavigationGrid(void)
{
	m_rows  = m_cols   = 0;
	m_offX  = m_offZ   = 0.0f;
	m_sizeX = m_sizeZ  = 1.0f;
}

NavigationGrid::~NavigationGrid(void)
{
}

void NavigationGrid::init(const std::vector<std::vector<int> >& grid, 
						  float xmin, float zmin, float xmax, float zmax) 
{
	if (grid.size() == 0 || grid[0].size() == 0) return;

	m_rows  = grid.size();
	m_cols  = grid[0].size();
	m_offX  = xmin;
	m_offZ  = zmin;
	m_sizeX = (xmax - xmin)/m_cols;
	m_sizeZ = (zmax - zmin)/m_rows;
	m_grid  = grid;
}

bool NavigationGrid::walkable(const Vec3d& pos) const 
{
	float fcol = (pos[0] - m_offX)/m_sizeX;
	float frow = (pos[2] - m_offZ)/m_sizeZ;
	if (fcol < 0.0f || frow < 0.0f) return false;
	return walkable(int(frow), int(fcol));
}

bool NavigationGrid::walkable(int row, int col) const
{
	if (row < 0 || row >= m_rows) return false;
	if (col < 0 || col >= m_cols) return false;
	return m_grid[row][col] == 0;
}

float NavigationGrid::cost(int cellFrom, int cellTo) const
{
	int c0 = cellFrom%m_cols;
	int r0 = cellFrom/m_cols;
	int c1 = cellTo  %m_cols;
	int r1 = cellTo  /m_cols;
	return std::sqrt(float((r1 - r0)*(r1 - r0) + (c1 - c0)*(c1 - c0)));
}

float NavigationGrid::heuristic(int cellFrom, int cellTo) const
{
	int c0 = cellFrom%m_cols;
	int r0 = cellFrom/m_cols;
	int c1 = cellTo  %m_cols;
	int r1 = cellTo  /m_cols;
	return std::sqrt(float((r1 - r0)*(r1 - r0) + (c1 - c0)*(c1 - c0)));
}

bool NavigationGrid::findPath(const Vec3d& ini, const Vec3d& end, std::vector<Vec3d>& path) 
{
	int iniCol  = int((ini[0] - m_offX)/m_sizeX);
	int iniRow  = int((ini[2] - m_offZ)/m_sizeZ);
	int iniCell = iniRow*m_cols + iniCol;
	int endCol  = int((end[0] - m_offX)/m_sizeX);
	int endRow  = int((end[2] - m_offZ)/m_sizeZ);
	int endCell = endRow*m_cols + endCol;

	if (!walkable(iniRow, iniCol)) return false;
	if (!walkable(endRow, endCol)) return false;
	
	bool arrived = iniCell == endCell;
	std::vector<float> cellCost(m_rows*m_cols, 1.0e20f);
	std::vector<int>   previous(m_rows*m_cols, -1);

	std::priority_queue<ANode> PQ;
	PQ.push(ANode(heuristic(iniCell, endCell), Move(0.0f, CellMove(iniCell, iniCell))));
	cellCost[iniCell] = 0;

	while (!arrived && !PQ.empty()) {

		// remove top
		ANode current = PQ.top();
		PQ.pop();

		// get movement info
		Move  move   =  current.second;
		int moveCost =  move.first;
		int cellFrom =  move.second.first;
		int cellTo   =  move.second.second;

		float tcost = moveCost;
		int   ccell = cellTo;
		int   crow  = ccell/m_cols;
		int   ccol  = ccell%m_cols;

		// if better path to current cell, update info
		if (cellCost[ccell] > tcost) {
			cellCost[ccell] = tcost;
			previous[ccell] = cellFrom;
		}

		// end condition
		if (ccell == endCell) {
			arrived = true;
		}
		else {
			// insert the new nodes
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) continue;
					if (walkable(crow + i, ccol + j)) {
						int destCell = (crow + i)*m_cols + ccol + j;
						float c = cost(ccell, destCell);
						float g = tcost + c;
						float h = heuristic(destCell, endCell);
						if (g < cellCost[destCell]) {
							ANode nnode;
							nnode.first  = -(g + h);
							nnode.second = Move(g, CellMove(ccell, destCell));
							PQ.push(nnode);
						}
					}
				}
			}
		}

	}

	// if arrived, reconstruct path
	if (arrived) {
		path.push_back(end);
		int vcell = previous[endCell];
		while (vcell >= 0 && vcell != iniCell) {
			int vr = vcell/m_cols;
			int vc = vcell%m_cols;
			Vec3d vp(m_offX + (vc + 0.5)*m_sizeX, 0, m_offZ + (vr + 0.5)*m_sizeZ);
			path.insert(path.begin(), vp);
			vcell = previous[vcell];
		}
	}

	return arrived;
}

void NavigationGrid::getObstacleAreas(std::vector<Vec4d>& rects) const
{
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			if (!walkable(i, j)) {
				Vec4d q;
				q[0] = j*m_sizeX + m_offX;
				q[1] = i*m_sizeZ + m_offZ;
				q[2] = (j+1)*m_sizeX + m_offX;
				q[3] = (i+1)*m_sizeZ + m_offZ;
				rects.push_back(q);
			}
		}
	}
}

Vec4d NavigationGrid::getArea() const 
{
	Vec4d a;
	a[0] = m_offX;
	a[1] = m_offZ;
	a[2] = m_offX + m_cols*m_sizeX;
	a[3] = m_offZ + m_rows*m_sizeZ;
	return a;
}

void NavigationGrid::getCells(int& rows, int& cols) const 
{
	rows = m_rows;
	cols = m_cols;
}
