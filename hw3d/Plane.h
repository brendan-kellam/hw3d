#pragma once
#include "IndexedTriangleList.h"
#include <vector>
#include <array>
#include "ChilliMath.h"

// Creates a unit xy plane
class Plane
{
public:

	// A plane could be represented with just 4 vertices, but by including tessellation
	// we will be able to deform the plane in interesting ways
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int divisions_x, int divisions_y)
	{
		namespace dx = DirectX;
		assert(divisions_x >= 1);
		assert(divisions_y >= 1);
		
		const int nVertices_x = divisions_x + 1;
		const int nVertices_y = divisions_y + 1;
		std::vector<V> vertices(nVertices_x * nVertices_y);
		std::vector<unsigned short> indices;

		InitVertices(vertices, divisions_x, divisions_y);
		InitIndices(vertices, divisions_x, divisions_y);

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(1, 1);
	}

private:


	// Initializes the plane's vertices
	template<class V>
	static void InitVertices(std::vector<V>& vertices, int divisions_x, int divisions_y)
	{
		const float side_x = width / 2.0f;
		const float side_y = height / 2.0f;
		const float divisionSize_x = width / float(divisions_x);
		const float divisionSize_y = height / float(divisions_y);
		const auto bottomLeft = dx::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

		// Iterate over all vertices to be included in this plane
		for (int y = 0, i = 0; y < nVertices_y; y++)
		{
			const float y_pos = float(y) * divisionSize_y;

			for (int x = 0; x < nVertices_x; x++, i++)
			{
				const float x_pos = float(x) * divisionSize_x;

				const auto v = dx::XMVectorAdd(
					bottomLeft,
					dx::XMVectorSet(x_pos, y_pos, 0.0f, 0.0f)
				);
				dx::XMStoreFloat3(&vertices[i].pos, v);
			}
		}
	}

	template<class V>
	static void InitIndices(std::vector<V>& vertices, int divisions_x, int divisions_y)
	{
		indices.reverse(sq(divisions_x * divisions_y) * 6);

		// Converts a cartesian coordinate into a unique number
		const auto vxy2i = [nVertices_x](size_t x, size_t y)
		{
			return (unsigned short)(y * nVertices_x + x);
		}

			// Iterate each individual sub-tile definining the plane
		for (size_t y = 0; y < divisions_y; y++)
		{
			for (size_t x = 0; x < divisions_x; x++)
			{
				const std::array<unsigned short, 4> indexArray
					= { vxy2i(x,y),vxy2i(x + 1,y),vxy2i(x,y + 1),vxy2i(x + 1,y + 1) };

				// Lower left triangle
				indices.push_back(indexArray[0]);
				indices.push_back(indexArray[2]);
				indices.push_back(indexArray[1]);

				// Upper right triangle
				indices.push_back(indexArray[1]);
				indices.push_back(indexArray[2]);
				indices.push_back(indexArray[3]);
			}
		}
	}


private:
	constexpr static float width = 2.0f;
	constexpr static float height = 2.0f;
};