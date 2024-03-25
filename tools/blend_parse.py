import bmesh
import bpy

f = open("../g_models.ij", "w")

f.write("#ifndef __G_MODELS_IJ__\n")
f.write("#define __G_MODELS_IJ__\n\n")
f.write("// === g_models.ij ===\n\n")

f.write("#include <vector.ij>\n")
f.write("#include <face.ij>\n\n")

for object in bpy.data.objects:
    if object.type != "MESH":
        continue
    
    mesh = object.data
    
    b_mesh = bmesh.new()
    b_mesh.from_mesh(mesh)

    bmesh.ops.triangulate(b_mesh, faces=b_mesh.faces[:])
    b_mesh.to_mesh(mesh)

    print(object.name)

    f.write("const face_t g_" + object.name + "[] = {\n")
    
    for poly in mesh.polygons:
        material = object.material_slots[poly.material_index].material
        nodes = material.node_tree.nodes

        principled = next(n for n in nodes if n.type == "BSDF_PRINCIPLED")
        
        stl_normal = poly.normal
        stl_face_a = mesh.vertices[poly.vertices[0]].co
        stl_face_b = mesh.vertices[poly.vertices[1]].co
        stl_face_c = mesh.vertices[poly.vertices[2]].co
        stl_color = principled.inputs["Base Color"].default_value

        f.write("  new face_t(" + str(stl_color[0] * 255.0) + ", " + str(stl_color[1] * 255.0) + ", " + str(stl_color[2] * 255.0) + ", ")
        f.write("new vector_t(" + str(stl_normal.x) + ", " + str(stl_normal.z) + ", " + str(stl_normal.y) + "), ")
        f.write("new vector_t(" + str(stl_face_a.x) + ", " + str(stl_face_a.z) + ", " + str(stl_face_a.y) + "), ")
        f.write("new vector_t(" + str(stl_face_b.x) + ", " + str(stl_face_b.z) + ", " + str(stl_face_b.y) + "), ")
        f.write("new vector_t(" + str(stl_face_c.x) + ", " + str(stl_face_c.z) + ", " + str(stl_face_c.y) + ")),\n")

    f.write("};\n\n");

f.write("// ^^^ g_models.ij ^^^\n\n")
f.write("#endif\n")
