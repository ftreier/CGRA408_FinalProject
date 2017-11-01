import maya.cmds as cmds


file_path = "D:/Documents/Victoria/2017/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"


def export_selected():
    selected_mesh = cmds.ls(selection=True)[0]

    if selected_mesh == None:
        cmds.error("Please select a triangle mesh object!")

    # Get the number of vertices and faces
    num_vertices = cmds.polyEvaluate(vertex=True)
    num_faces = cmds.polyEvaluate(face=True)

    cmds.select(selected_mesh + ".f[0: " + str(num_faces) + "]")

    # Get the vertex indices per face
    face_vertex_indices = cmds.polyInfo(faceToVertex=True)

    for f in range(0, num_faces):
        tokens = face_vertex_indices[f].split()[2:]

        if len(tokens) != 3:
            cmds.error("Selected object must be a triangle mesh!")

        face_vertex_indices[f] = map(int, tokens)

    # Get the vertex positions and vertex normals per face
    face_vertex_positions = []
    face_vertex_normals = []
    vertex_positions = cmds.xform(selected_mesh + ".vtx[0:" + str(num_vertices) + "]", query=True, objectSpace=True, translation=True)

    for f in range(0, num_faces):
        face_vertex_positions.append([])
        face_vertex_normals.append([])

        for v in face_vertex_indices[f]:
            face_vertex_positions[f].append(round(vertex_positions[v], 6))
            face_vertex_positions[f].append(round(vertex_positions[v + 1], 6))
            face_vertex_positions[f].append(round(vertex_positions[v + 2], 6))

            cmds.select(selected_mesh + ".vtx[" + str(v) + "]")
            vertex_normal = cmds.polyNormalPerVertex(query=True, xyz=True)[0:3]
            face_vertex_normals[f].append(round(vertex_normal[0], 6))
            face_vertex_normals[f].append(round(vertex_normal[1], 6))
            face_vertex_normals[f].append(round(vertex_normal[2], 6))

    # Reselect the original object
    cmds.select(selected_mesh)

    print vertex_positions

    # Write the mesh data to a pbrt file
    with open(file_path + selected_mesh + ".pbrt", "w") as file:

        # Write object transform
        translate = cmds.getAttr(selected_mesh + ".translate")[0]
        rotate = cmds.getAttr(selected_mesh + ".rotate")[0]
        scale = cmds.getAttr(selected_mesh + ".scale")[0]

        file.write("Translate {0:.6f} {1:.6f} {2:.6f}\n".format(translate[0], translate[1], translate[2]))
        file.write("Rotate {0:.6f} 1 0 0\n".format(rotate[0]))
        file.write("Rotate {0:.6f} 0 1 0\n".format(rotate[1]))
        file.write("Rotate {0:.6f} 0 0 1\n".format(rotate[2]))
        file.write("Scale {0:.6f} {1:.6f} {2:.6f}\n".format(scale[0], scale[1], scale[2]))

        # Write vertex positions and normals per face
        for f in range(0, num_faces):
            vertex_position_string = " ".join(map(str, face_vertex_positions[f]))
            vertex_normal_string = " ".join(map(str, face_vertex_normals[f]))
            file.write("Shape \"trianglemesh\"  \"integer indices\" [0 2 1] \"point P\" [{0}] \"normal N\" [{1}]\n".format(vertex_position_string, vertex_normal_string))


if __name__ == '__main__':
    print("Begin object export")
    export_selected()
    print("Finish object export")
