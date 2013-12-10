#ifndef _NAVIGATION_GRID_
#define _NAVIGATION_GRID_

#include <vector>
#include "defines.h"

class NavigationGrid
{

public:
	NavigationGrid(void);
	~NavigationGrid(void);

	void init(const std::vector<std::vector<int> >& grid, 
			  float xmin, float zmin, float xmax, float zmax);
	bool findPath(const Vec3d& ini, const Vec3d& end, std::vector<Vec3d>& path);

	bool walkable(const Vec3d& pos) const;

	void  getObstacleAreas(std::vector<Vec4d>& rects) const;
	Vec4d getArea() const;
	void  getCells(int& rows, int& cols) const;

private:

	bool  walkable(int row, int col) const;
	float cost(int cellFrom, int cellTo) const;
	float heuristic(int cellFrom, int cellTo) const;

	std::vector<std::vector<int> > m_grid;
	int		m_rows,  m_cols;
	float	m_offX,  m_offZ;
	float	m_sizeX, m_sizeZ;

};

#endif