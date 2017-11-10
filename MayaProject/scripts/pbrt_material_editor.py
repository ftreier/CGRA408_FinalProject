from PySide2 import QtCore, QtWidgets, QtGui
import os


class PBRTEditor(QtWidgets.QWidget):

    default_pbrt_exe_path = "~/Cgra408/FinalProject/CGRA408_FinalProject/pbrt-v3/cmake-build-release/pbrt"
    default_dir_path = "~/Cgra408/FinalProject/CGRA408_FinalProject/MayaProject/scripts"

    def __init__(self):
        super(PBRTEditor, self).__init__()

        # Setup GUI window
        self.setWindowTitle("PBRT Material Editor")
        self.layout = QtWidgets.QVBoxLayout(self)

        # Setup path inputs
        self.path_layout = QtWidgets.QFormLayout()
        self.path_layout.setLabelAlignment(QtCore.Qt.AlignLeft)

        self.pbrt_exe_path_input = QtWidgets.QLineEdit(self.default_pbrt_exe_path)
        self.path_layout.addRow("PBRT Executable Path:", self.pbrt_exe_path_input)
        self.dir_path_input = QtWidgets.QLineEdit(self.default_dir_path)
        self.path_layout.addRow("Material Editor Path:", self.dir_path_input)

        self.layout.addLayout(self.path_layout)

        # Setup image viewer
        self.image = QtGui.QPixmap()
        self.image_label = QtWidgets.QLabel()
        self.image_label.setPixmap(self.image)
        self.layout.addWidget(QtWidgets.QLabel("Sample"))
        self.layout.addWidget(self.image_label)
        self.layout.addWidget(QHLine())

        # Setup apply button
        self.updateButton = QtWidgets.QPushButton("Update")
        self.updateButton.clicked.connect(self.apply)
        self.layout.addWidget(self.updateButton)
        self.layout.addWidget(QHLine())

        # Setup material settings
        self.layout.addWidget(QtWidgets.QLabel("Plastic Settings"))
        self.plastic_settings = PlasticMaterialSettings(self)
        self.layout.addWidget(self.plastic_settings)
        self.layout.addWidget(QtWidgets.QLabel("Translucent Settings"))
        self.translucent_settings = TranslucentMaterialSettings(self)
        self.layout.addWidget(self.translucent_settings)
        self.layout.addWidget(QtWidgets.QLabel("Mix Settings"))
        self.mix_settings = MixMaterialSettings(self)
        self.layout.addWidget(self.mix_settings)
        self.layout.addWidget(QHLine())

        # Setup window
        self.resize(256, 256)

    @QtCore.Slot()
    def apply(self):
        self.toggle_loading_indicator(True)

        # Setup paths
        pbrt_exe_path = self.pbrt_exe_path_input.text()
        pbrt_template_file_path = self.dir_path_input.text() + "/template.pbrt"
        pbrt_editor_file_path = self.dir_path_input.text() + "/editor.pbrt"
        pbrt_editor_image_path = self.dir_path_input.text() + "/materialPreview.png"

        # Load PBRT template file
        template = self.read_pbrt_tempate_string(pbrt_template_file_path)

        # Replace template variables with UI data
        template = self.write_data_to_template_string(template, "plastic", self.plastic_settings.get_data())
        template = self.write_data_to_template_string(template, "translucent", self.translucent_settings.get_data())
        template = self.write_data_to_template_string(template, "mix", self.mix_settings.get_data())

        # Export PBRT file
        self.write_template_string_to_file(template, pbrt_editor_file_path)

        # Run PBRT render job
        command = pbrt_exe_path +\
                  " " + pbrt_editor_file_path# +\
                  #" --outfile " + pbrt_editor_image_path
        os.system(command)

        # Load resulting image into UI
        self.image.load(pbrt_editor_image_path.replace("~/", ""))
        self.image_label.setPixmap(self.image)

        self.toggle_loading_indicator(False)

    def toggle_loading_indicator(self, state):
        if state:
            self.updateButton.setText("Rendering...")
        else:
            self.updateButton.setText("Update")
        self.update()
        self.repaint()

    def read_pbrt_tempate_string(self, pbrt_template_file_path):
        with open(pbrt_template_file_path.replace("~/", ""), "r") as template_file:
            return template_file.read()

    def write_data_to_template_string(self, template, namespace, data):
        for key, value in data.items():
            template = template.replace("%%_" + namespace + "_" + key + "_%%", value)
        return template

    def write_template_string_to_file(self, template, pbrt_editor_file_path):
        with open(pbrt_editor_file_path.replace("~/", ""), "w") as editor_file:
            editor_file.write(template)


class PlasticMaterialSettings(QtWidgets.QWidget):

    def __init__(self, window):
        super(PlasticMaterialSettings, self).__init__()

        self.window = window
        self.layout = QtWidgets.QFormLayout(self)
        self.layout.setLabelAlignment(QtCore.Qt.AlignLeft)

        # Inputs
        self.diffuse = QtWidgets.QLineEdit("0.75 0.75 0.75")
        self.layout.addRow("Diffuse:", self.diffuse)

        self.specular = QtWidgets.QLineEdit("0.25 0.25 0.25")
        self.layout.addRow("Specular:", self.specular)

        self.roughness = QtWidgets.QLineEdit("0.1")
        self.roughness.setValidator(QtGui.QDoubleValidator())
        self.layout.addRow("Roughness:", self.roughness)

    def get_data(self):
        return {
            "Kd": self.diffuse.text(),
            "Ks": self.specular.text(),
            "roughness": self.roughness.text(),
        }


class TranslucentMaterialSettings(QtWidgets.QWidget):

    def __init__(self, window):
        super(TranslucentMaterialSettings, self).__init__()

        self.window = window
        self.layout = QtWidgets.QFormLayout(self)
        self.layout.setLabelAlignment(QtCore.Qt.AlignLeft)

        # Inputs
        self.diffuse = QtWidgets.QLineEdit("0.75 0.75 0.75")
        self.layout.addRow("Diffuse:", self.diffuse)

        self.specular = QtWidgets.QLineEdit("0.25 0.25 0.25")
        self.layout.addRow("Specular:", self.specular)

        self.roughness = QtWidgets.QLineEdit("0.1")
        self.roughness.setValidator(QtGui.QDoubleValidator())
        self.layout.addRow("Roughness:", self.roughness)

        self.reflect = QtWidgets.QLineEdit("0.5 0.5 0.5")
        self.layout.addRow("Reflect:", self.reflect)

        self.transmit = QtWidgets.QLineEdit("0.5 0.5 0.5")
        self.layout.addRow("Transmit:", self.transmit)

    def get_data(self):
        return {
            "Kd": self.diffuse.text(),
            "Ks": self.specular.text(),
            "roughness": self.roughness.text(),
            "reflect": self.reflect.text(),
            "transmit": self.transmit.text(),
        }


class MixMaterialSettings(QtWidgets.QWidget):

    def __init__(self, window):
        super(MixMaterialSettings, self).__init__()

        self.window = window
        self.layout = QtWidgets.QFormLayout(self)
        self.layout.setLabelAlignment(QtCore.Qt.AlignLeft)

        # Inputs
        self.mix = QtWidgets.QLineEdit("0.5 0.5 0.5")
        self.layout.addRow("Amount:", self.mix)

    def get_data(self):
        return {
            "mix": self.mix.text(),
        }


class QHLine(QtWidgets.QFrame):

    def __init__(self):
        super(QHLine, self).__init__()
        self.setFrameShape(QtWidgets.QFrame.HLine)
        self.setFrameShadow(QtWidgets.QFrame.Sunken)


if __name__ == '__main__':
    editorWindow = PBRTEditor()
    editorWindow.show()
