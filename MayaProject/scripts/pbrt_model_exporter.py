import maya.cmds as cmds


file_path = "/home/purvisjack/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"
# file_path = "D:/Documents/Victoria/2017/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"


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

    # Flatten the face vertex indices
    face_vertex_indices = sum(face_vertex_indices, [])

    # Get the vertex positions
    vertex_positions = cmds.xform(selected_mesh + ".vtx[0:" + str(num_vertices) + "]", query=True, objectSpace=True, translation=True)
    vertex_normals = []

    for v in range(0, num_vertices):

        # Round the vertex position
        vertex_positions[v*3] = round(vertex_positions[v*3], 6)
        vertex_positions[v*3+1] = round(vertex_positions[v*3+1], 6)
        vertex_positions[v*3+2] = round(vertex_positions[v*3+2], 6)

        # Get the vertex normal for this vertex
        cmds.select(selected_mesh + ".vtx[" + str(v) + "]")
        vertex_normal = cmds.polyNormalPerVertex(query=True, xyz=True)[0:3]
        vertex_normals.append(round(vertex_normal[0], 6))
        vertex_normals.append(round(vertex_normal[1], 6))
        vertex_normals.append(round(vertex_normal[2], 6))

    # Reselect the original object
    cmds.select(selected_mesh)

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

        # Write vertex indices, positions and normals per face
        vertex_indices_string = " ".join(map(str, face_vertex_indices))
        vertex_position_string = " ".join(map(str, vertex_positions))
        vertex_normal_string = " ".join(map(str, vertex_normals))
        file.write("Shape \"trianglemesh\"  \"integer indices\" [{0}] \"point P\" [{1}] \"normal N\" [{2}]\n".format(vertex_indices_string, vertex_position_string, vertex_normal_string))


if __name__ == '__main__':
    print("Begin object export")
    export_selected()
    print("Finish object export")
