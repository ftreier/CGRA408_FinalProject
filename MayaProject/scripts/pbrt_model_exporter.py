import maya.cmds as cmds


#file_path = "/home/purvisjack/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"
file_path = "D:/Documents/Victoria/2017/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"


def export_selected():
    selected_mesh = cmds.ls(selection=True)[0]

    # Create a triangle mesh duplicate of the selected mesh
    triangle_mesh = cmds.duplicate()[0]
    cmds.polyTriangulate()
    cmds.select(triangle_mesh)

    if triangle_mesh == None:
        cmds.error("Please select a triangle mesh object!")

    # Get the number of vertices and faces
    num_vertices = cmds.polyEvaluate(vertex=True)
    num_faces = cmds.polyEvaluate(face=True)

    cmds.select(triangle_mesh + ".f[0: " + str(num_faces) + "]")

    # Get the vertex indices per face
    face_vertex_indices = cmds.polyInfo(faceToVertex=True)

    for f in range(0, num_faces):
        tokens = face_vertex_indices[f].split()[2:]

        if len(tokens) != 3:
            cmds.error("Selected object must be a triangle mesh!")

        face_vertex_indices[f] = map(int, tokens)

    # Flatten the face vertex indices
    face_vertex_indices = sum(face_vertex_indices, [])

    # Get the vertex positions
    vertex_positions = cmds.xform(triangle_mesh + ".vtx[0:" + str(num_vertices) + "]", query=True, objectSpace=True, translation=True)
    vertex_normals = []

    for v in range(0, num_vertices):

        # Round the vertex position
        vertex_positions[v*3] = round(vertex_positions[v*3], 6)
        vertex_positions[v*3+1] = round(vertex_positions[v*3+1], 6)
        vertex_positions[v*3+2] = round(vertex_positions[v*3+2], 6)

        # Get the vertex normal for this vertex
        cmds.select(triangle_mesh + ".vtx[" + str(v) + "]")
        all_vertex_normals = cmds.polyNormalPerVertex(query=True, xyz=True)
        vertex_normal_x = (all_vertex_normals[0] + all_vertex_normals[3] + all_vertex_normals[6] + all_vertex_normals[9]) / 4
        vertex_normal_y = (all_vertex_normals[1] + all_vertex_normals[4] + all_vertex_normals[7] + all_vertex_normals[10]) / 4
        vertex_normal_z = (all_vertex_normals[2] + all_vertex_normals[5] + all_vertex_normals[8] + all_vertex_normals[11]) / 4

        vertex_normals.append(round(vertex_normal_x, 6))
        vertex_normals.append(round(vertex_normal_y, 6))
        vertex_normals.append(round(vertex_normal_z, 6))

    # Write the mesh data to a pbrt file
    with open(file_path + selected_mesh + ".pbrt", "w") as file:

        # Write object transform
        translate = cmds.getAttr(triangle_mesh + ".translate")[0]
        rotate = cmds.getAttr(triangle_mesh + ".rotate")[0]
        scale = cmds.getAttr(triangle_mesh + ".scale")[0]

        file.write("Translate {0:.6f} {1:.6f} {2:.6f}\n".format(translate[0], translate[1], translate[2]))
        file.write("Rotate {0:.6f} 1 0 0\n".format(rotate[0]))
        file.write("Rotate {0:.6f} 0 1 0\n".format(rotate[1]))
        file.write("Rotate {0:.6f} 0 0 1\n".format(rotate[2]))
        file.write("Scale {0:.6f} {1:.6f} {2:.6f}\n".format(scale[0], scale[1], scale[2]))

        # Write vertex indices, positions and normals per face
        vertex_indices_string = " ".join(map(str, face_vertex_indices))
        vertex_position_string = " ".join(map(str, vertex_positions))
        vertex_normal_string = " ".join(map(str, vertex_normals))
        file.write("Shape \"trianglemesh\"  \"integer indices\" [{0}] \"point P\" [{1}] \"normal N\" [{2}]\n".format(vertex_indices_string, vertex_position_string, vertex_normal_string))


    # Destory the duplicate triangle mesh
    cmds.select(triangle_mesh)
    cmds.delete()

if __name__ == '__main__':
    print("Begin object export")
    export_selected()
    print("Finish object export")
