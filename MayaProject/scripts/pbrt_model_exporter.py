import maya.cmds as cmds


#file_path = "/home/purvisjack/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"
file_path = "D:/Documents/Victoria/2017/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"


def export_selected():
    selected_mesh = cmds.ls(selection=True)[0]

    # Check if the selected object is a group containing many child objects
    children = cmds.listRelatives(selected_mesh, type="transform")
    if children != None and len(children) > 0:

        # Export all the child objects
        for child in children:
            export_mesh(child, selected_mesh)

        # Write a pbrt file that includes all of the child object pbrt files
        with open(file_path + selected_mesh + ".pbrt", "w") as file:
            for child in children:
                file.write("AttributeBegin\n\tInclude \"" + selected_mesh + "/" + child + ".pbrt\"\nAttributeEnd\n\n")
    else:
        # Export the single selected object
        export_mesh(selected_mesh)


def export_mesh(selected_mesh, parent_name=None):
    cmds.select(selected_mesh)

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
    vertex_positions = cmds.xform(triangle_mesh + ".vtx[0:" + str(num_vertices) + "]", query=True, translation=True)
    vertex_normals = []
    vertex_uvs = []

    for v in range(0, num_vertices):

        # Round the vertex position
        vertex_positions[v*3] = round(vertex_positions[v*3], 6)
        vertex_positions[v*3+1] = round(vertex_positions[v*3+1], 6)
        vertex_positions[v*3+2] = round(vertex_positions[v*3+2], 6)

        # Get the vertex normal for this vertex
        cmds.select(triangle_mesh + ".vtx[" + str(v) + "]")
        all_vertex_normals = cmds.polyNormalPerVertex(query=True, xyz=True)
        if len(all_vertex_normals) >= 12:
            vertex_normal_x = (all_vertex_normals[0] + all_vertex_normals[3] + all_vertex_normals[6] + all_vertex_normals[9]) / 4
            vertex_normal_y = (all_vertex_normals[1] + all_vertex_normals[4] + all_vertex_normals[7] + all_vertex_normals[10]) / 4
            vertex_normal_z = (all_vertex_normals[2] + all_vertex_normals[5] + all_vertex_normals[8] + all_vertex_normals[11]) / 4
        else:
            # Naive handling of vertices with less than 4 vertex normals
            vertex_normal_x = 0.0
            vertex_normal_y = 0.0
            vertex_normal_z = 0.0

        vertex_normals.append(round(vertex_normal_x, 6))
        vertex_normals.append(round(vertex_normal_y, 6))
        vertex_normals.append(round(vertex_normal_z, 6))

        # Get the UV co-ordinates for this vertex
        uv_index = cmds.polyListComponentConversion(triangle_mesh + ".vtx[" + str(v) + "]", fromVertex=True, toUV=True)[0]
        cmds.select(uv_index)
        uv = cmds.polyEditUV(query=True)
        vertex_uvs.append(round(uv[0], 6))
        vertex_uvs.append(round(uv[1], 6))


    # If the object is part of a group it should be stored in a sub-directory of the parent name
    file_name = file_path
    if parent_name != None:
        file_name = file_name + parent_name + "/" + selected_mesh + ".pbrt"
    else:
        file_name = file_name + selected_mesh + ".pbrt"

    # Write the mesh data to a pbrt file
    with open(file_name, "w") as file:

        # Write parent transform if object has a transform parent
        parents = cmds.listRelatives(selected_mesh, parent=True, type="transform")
        if len(parents) > 0:
            matrix = cmds.xform(parents[0], query=True, matrix=True)
            for m in range(0, len(matrix)):
                matrix[m] = round(matrix[m], 6)
            file.write("ConcatTransform [{0}]\n".format(" ".join(map(str, matrix))))

        # Write object transform
        matrix = cmds.xform(triangle_mesh, query=True, matrix=True)
        for m in range(0, len(matrix)):
            matrix[m] = round(matrix[m], 6)
        file.write("ConcatTransform [{0}]\n\n".format(" ".join(map(str, matrix))))

        # Write vertex indices, positions and normals per face
        vertex_indices_string = " ".join(map(str, face_vertex_indices))
        vertex_position_string = " ".join(map(str, vertex_positions))
        vertex_normal_string = " ".join(map(str, vertex_normals))
        vertex_uv_string = " ".join(map(str, vertex_uvs))
        file.write("Shape \"trianglemesh\"\n\t\"integer indices\" [{0}]\n\t\"point P\" [{1}]\n\t\"normal N\" [{2}]\n\t\"float uv\" [{3}]\n".format(vertex_indices_string, vertex_position_string, vertex_normal_string, vertex_uv_string))


    # Destory the duplicate triangle mesh
    cmds.select(triangle_mesh)
    cmds.delete()


if __name__ == '__main__':
    print("Begin object export")
    export_selected()
    print("Finish object export")
