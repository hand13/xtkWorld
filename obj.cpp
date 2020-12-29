//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <string>
#include <vector>
#include <iostream>
#include "obj.h"
std::vector<ObjShape> loadFromFile(std::string fileName) {
  std::vector<ObjShape> result;
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "./";
  tinyobj::ObjReader reader;
  if(!reader.ParseFromFile(fileName,reader_config)) {
    if(!reader.Error().empty()) {
      std::cerr<<"read obj failed"<<reader.Error()<<std::endl;
    }
    return result;
  }
  if(!reader.Warning().empty()) {
    std::cout<<"read warning "<<reader.Warning()<<std::endl;
  }
  auto& attrib = reader.GetAttrib();
  auto& shapes = reader.GetShapes();
  auto & materials = reader.GetMaterials();
  std::cout<<"vetrices size is "<<attrib.vertices.size()<<std::endl;
  std::cout<<"shapes size is "<<shapes.size()<<std::endl;
  for(size_t s=0;s < shapes.size();s++) {
    ObjShape os;
    size_t index_offset =0;
    std::cout<<"faces size is "<<shapes[s].mesh.num_face_vertices.size()<<std::endl;
    for(size_t f =0;f<shapes[s].mesh.num_face_vertices.size();f++) {
      unsigned int fv = shapes[s].mesh.num_face_vertices[f];
      for(size_t v=0;v<fv;v++) {
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
        tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
        tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
        tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
        tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
        tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
        tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index+ 0];
        tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index+ 1];
        os.vertices.push_back(vx);
        os.vertices.push_back(vy);
        os.vertices.push_back(vz);
        os.vertices.push_back(tx);
        os.vertices.push_back(ty);
      }
      index_offset +=fv;
      auto m = shapes[s].mesh.material_ids[f];
    }
    result.push_back(os);
  }
  return result;
}
