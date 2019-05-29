#pragma once


/*
Container for a set of vertices and indices representing a collection of triangles
*/
template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	
	// Note: Type T must contain a member pos
	IndexedTriangleList(std::vector<T> vertsIn, std::vector<unsigned short> indicesIn)
		: vertices(std::move(vertsIn)),
		indices(std::move(indicesIn))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix) 
	{
		// Iterate over each vertex
		for (auto& v : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix));
		}
	}

private:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};