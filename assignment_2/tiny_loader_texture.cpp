/* tiny_loader_texture.cpp
Example class to demonstrate the use of TinyObjectLoader to load an obj (WaveFront)
object file with normals and texture coordinates, and copy the data into vertex, normal texture coordinate buffers.
A colour buffer is not included as it is expected that the colour be taken form the texture.
Please be careful to match the vertex attribute indices in your shaders. See code in the
constructor:

	attribute_v_coord = 0;
	attribute_v_normal = 1;
	attribute_v_texcoord = 2;

Iain Martin November 2018
*/

#include "tiny_loader_texture.h"
#include <iostream>
#include <stdio.h>

//Tinyobjloader library used to import models
#ifndef TINYOBJLOADER_IMPLEMENTATION
	#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
	#include "tiny_obj_loader.h"
#endif

using namespace std;
using namespace glm;

// Debig print method to print out the attributres loaded from the obj file
static  void PrintInfo(const tinyobj::attrib_t& attrib,
	const vector<tinyobj::shape_t>& shapes,
	const vector<tinyobj::material_t>& materials); 

TinyObjLoader::TinyObjLoader()
{
	attribute_v_coord = 0;
	attribute_v_normal = 2;
	attribute_v_texcoord = 3;

	numVertices = 0;
	numNormals = 0;
	numTexCoords = 0;
}

TinyObjLoader::~TinyObjLoader()
{
}


void TinyObjLoader::load_obj(string inputfile, bool debugPrint)
{
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;


	string err, warn;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &warn, inputfile.c_str());

	if (!err.empty()) { // `err` may contain error messages.
		cerr << err << endl;
	}
	
	if (!err.empty()) { // `warn` may contain warning messages.
		cerr << err << endl;
	}

	if (!ret) {
		exit(1);
	}

	// Calculate the number of vertices from the shapes
	numVertices = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		numVertices += shapes[s].mesh.num_face_vertices.size() * 3;//3 vertexes for each face
	}

	numNormals = numTexCoords = numVertices;

	//have to duplicate vertices (glDrawElements not possible) because of texture
	std::vector<tinyobj::real_t> pVertices(numVertices * 3);
	std::vector<tinyobj::real_t> pTextureCoords(numVertices * 2);
	std::vector<tinyobj::real_t> pNormals(numVertices * 3);

	GLuint ind = 0;
	for (size_t s = 0; s < shapes.size(); s++) {

		// Loop over faces(polygon)
		size_t index_offset = 0;

		//number of faces; vert, text, norm
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
		{
			int fv = shapes[s].mesh.num_face_vertices[f];//number of vertices per face (3)

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) 
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				pVertices[ind * 3 + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				pVertices[ind * 3 + 1] = attrib.vertices[3 * idx.vertex_index + 1];
				pVertices[ind * 3 + 2] = attrib.vertices[3 * idx.vertex_index + 2];

				pTextureCoords[ind * 2 + 0] = attrib.texcoords[2 * idx.texcoord_index + 0];
				pTextureCoords[ind * 2 + 1] = attrib.texcoords[2 * idx.texcoord_index + 1];

				pNormals[ind * 3 + 0] = attrib.normals[3 * idx.normal_index + 0];
				pNormals[ind * 3 + 1] = attrib.normals[3 * idx.normal_index + 1];
				pNormals[ind * 3 + 2] = attrib.normals[3 * idx.normal_index + 2];

				ind++;
			}
			index_offset += fv;
		}
	}

	// Copy the vertix, normal and textcoord data into OpenGL buffers
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(tinyobj::real_t), &pVertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, pNormals.size() * sizeof(tinyobj::real_t), &pNormals.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &texCoordsObject);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsObject);
	glBufferData(GL_ARRAY_BUFFER, pTextureCoords.size() * sizeof(tinyobj::real_t), &pTextureCoords.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void TinyObjLoader::drawObject(int drawmode)
{

	/* Draw the object as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_coord);

	/* Bind the object normals */
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribute_v_normal);

	/* Bind the object texture coords if they exist */
	glEnableVertexAttribArray(attribute_v_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsObject);
	glVertexAttribPointer(attribute_v_texcoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, numVertices);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

static void PrintInfo(const tinyobj::attrib_t& attrib,
	const vector<tinyobj::shape_t>& shapes,
	const vector<tinyobj::material_t>& materials) {
	cout << "# of vertices  : " << (attrib.vertices.size() / 3) << endl;
	cout << "# of normals   : " << (attrib.normals.size() / 3) << endl;
	cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
		<< endl;

	cout << "# of shapes    : " << shapes.size() << endl;
	cout << "# of materials : " << materials.size() << endl;

	for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			static_cast<const double>(attrib.vertices[3 * v + 0]),
			static_cast<const double>(attrib.vertices[3 * v + 1]),
			static_cast<const double>(attrib.vertices[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
		printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			static_cast<const double>(attrib.normals[3 * v + 0]),
			static_cast<const double>(attrib.normals[3 * v + 1]),
			static_cast<const double>(attrib.normals[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
		printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
			static_cast<const double>(attrib.texcoords[2 * v + 0]),
			static_cast<const double>(attrib.texcoords[2 * v + 1]));
	}

	// For each shape
	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", static_cast<long>(i),
			shapes[i].name.c_str());
		printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].mesh.indices.size()));
		printf("Size of shape[%ld].path.indices: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].path.indices.size()));

		size_t index_offset = 0;

		assert(shapes[i].mesh.num_face_vertices.size() ==
			shapes[i].mesh.material_ids.size());

		assert(shapes[i].mesh.num_face_vertices.size() ==
			shapes[i].mesh.smoothing_group_ids.size());

		printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

		// For each face
		for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
			size_t fnum = shapes[i].mesh.num_face_vertices[f];

			printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
				static_cast<unsigned long>(fnum));

			// For each vertex in the face
			for (size_t v = 0; v < fnum; v++) {
				tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
				printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
					static_cast<long>(v), idx.vertex_index, idx.normal_index,
					idx.texcoord_index);
			}

			printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
				shapes[i].mesh.material_ids[f]);
			printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
				shapes[i].mesh.smoothing_group_ids[f]);

			index_offset += fnum;
		}

		printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].mesh.tags.size()));
		for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
			printf("  tag[%ld] = %s ", static_cast<long>(t),
				shapes[i].mesh.tags[t].name.c_str());
			printf(" ints: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
				printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
				if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");

			printf(" floats: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
				printf("%f", static_cast<const double>(
					shapes[i].mesh.tags[t].floatValues[j]));
				if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");

			printf(" strings: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
				printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
				if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");
			printf("\n");
		}
	}

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", static_cast<long>(i),
			materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n",
			static_cast<const double>(materials[i].ambient[0]),
			static_cast<const double>(materials[i].ambient[1]),
			static_cast<const double>(materials[i].ambient[2]));
		printf("  material.Kd = (%f, %f ,%f)\n",
			static_cast<const double>(materials[i].diffuse[0]),
			static_cast<const double>(materials[i].diffuse[1]),
			static_cast<const double>(materials[i].diffuse[2]));
		printf("  material.Ks = (%f, %f ,%f)\n",
			static_cast<const double>(materials[i].specular[0]),
			static_cast<const double>(materials[i].specular[1]),
			static_cast<const double>(materials[i].specular[2]));
		printf("  material.Tr = (%f, %f ,%f)\n",
			static_cast<const double>(materials[i].transmittance[0]),
			static_cast<const double>(materials[i].transmittance[1]),
			static_cast<const double>(materials[i].transmittance[2]));
		printf("  material.Ke = (%f, %f ,%f)\n",
			static_cast<const double>(materials[i].emission[0]),
			static_cast<const double>(materials[i].emission[1]),
			static_cast<const double>(materials[i].emission[2]));
		printf("  material.Ns = %f\n",
			static_cast<const double>(materials[i].shininess));
		printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
		printf("  material.dissolve = %f\n",
			static_cast<const double>(materials[i].dissolve));
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n",
			materials[i].specular_highlight_texname.c_str());
		printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
		printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
		printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
		printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
		printf("  <<PBR>>\n");
		printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
		printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
		printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
		printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
		printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
		printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
		printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
		printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
		printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
		printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
		printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
		printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
		map<string, string>::const_iterator it(
			materials[i].unknown_parameter.begin());
		map<string, string>::const_iterator itEnd(
			materials[i].unknown_parameter.end());

		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}
}