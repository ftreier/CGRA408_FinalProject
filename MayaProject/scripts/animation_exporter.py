import maya.cmds as cmds


file_path = "/home/purvisjack/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"
#file_path = "D:/Documents/Victoria/2017/Cgra408/FinalProject/CGRA408_FinalProject/scenes/"


def export_selected_animation():
    selected_mesh = cmds.ls(selection=True)[0]

    if selected_mesh == None:
        cmds.error("Please select an object!")


if __name__ == '__main__':
    print("Begin animation export")
    export_selected_animation()
    print("Finish animation export")
