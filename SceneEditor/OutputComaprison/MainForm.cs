using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Crom.Controls;
using OutputComaprison.Properties;

namespace OutputComaprison
{
	public partial class MainForm : Form
	{
		#region fields

		private readonly List<ImageBox> _imageBoxes;
		private readonly HashSet<DockableToolWindow> _updatingSize;
		private readonly Dictionary<string, zDockMode> _imageNames;
		private PbrtSceneEditor _editor;

		private string _pbrtPath;
		private string _baseImgPath;

		#endregion

		#region Properties

		public string PbrtPath
		{
			get { return _pbrtPath; }
			private set
			{
				if (string.Compare(_pbrtPath, value, StringComparison.InvariantCultureIgnoreCase) != 0 || string.IsNullOrEmpty(value))
				{
					_pbrtPath = value;
					UpdatePbrtPath();
				}
			}
		}

		#endregion

		#region constructor

		public MainForm()
		{
			InitializeComponent();

			_imageBoxes = new List<ImageBox>();
			_updatingSize = new HashSet<DockableToolWindow>();
			_imageNames = new Dictionary<string, zDockMode>
			{
				{"complete", zDockMode.Left},
				{"diff", zDockMode.Bottom},
				{"mask", zDockMode.Bottom},
				{"synthetic", zDockMode.Right},
				{"local", zDockMode.Right},
				{"environment", zDockMode.Left},
				{"final", zDockMode.Right}
			};

			_dockContainer.BottomPanelHeight = 400;
		}

		#endregion

		#region Create Windows

		private void CreateImageWindows(string fileName, zDockMode dockMode)
		{
			DockableToolWindow childForm = new DockableToolWindow();
			ImageBox ib = new ImageBox(_baseImgPath, fileName) {Dock = DockStyle.Fill};
			childForm.Controls.Add(ib);
			childForm.Text = ib.FileName;
			childForm.Width = 400;
			childForm.Height = 300;

			childForm.SizeChanged += ChildForm_SizeChanged;

			// Add the form to the dock container
			_dockContainer.DockToolWindow(childForm, dockMode);

			// Show the form
			childForm.Show();
			_imageBoxes.Add(ib);
			ib.ReloadImage();
		}

		private void CreateSceneEditor()
		{
			_editor = new PbrtSceneEditor() { Dock = DockStyle.Fill };
			DockableToolWindow childForm = new DockableToolWindow {Text = @"Scene Editor"};
			childForm.Controls.Add(_editor);

			// Add the form to the dock container
			_dockContainer.DockToolWindow(childForm, zDockMode.Fill);

			// Show the form
			childForm.Show();
		}

		#endregion

		private void ReloadImages()
		{
			UpdateBaseImgPath();
			foreach (var imageBox in _imageBoxes)
			{
				imageBox.ReloadImage();
			}
		}

		#region Update 

		private void UpdatePbrtPath()
		{
			_pbrtPathTb.Text = _pbrtPath;
			if (File.Exists(_pbrtPath))
			{
				_pbrtPathLbl.ForeColor = Color.Black;
				_pbrtPathTb.ForeColor = Color.Black;
				_run.Enabled = true;
			}
			else
			{
				_pbrtPathLbl.ForeColor = Color.Red;
				_pbrtPathTb.ForeColor = Color.Red;
				_run.Enabled = false;
			}
		}

		private void UpdateBaseImgPath()
		{
			_baseImgPathTb.Text = _baseImgPath;
			try
			{
				if (File.Exists(Path.Combine(_baseImgPath, _imageNames.First().Key + ".exr")) ||
				    File.Exists(Path.Combine(_baseImgPath, _imageNames.First().Key + ".png")))
				{
					_baseImgPathTb.ForeColor = Color.Black;
					_baseImgPathLbl.ForeColor = Color.Black;

					foreach (var imageBox in _imageBoxes)
					{
						imageBox.SetBasePath(_baseImgPath);
					}
				}
				else
				{
					_baseImgPathTb.ForeColor = Color.Red;
					_baseImgPathLbl.ForeColor = Color.Red;
				}
			}
			catch (Exception)
			{
				_baseImgPath = "";
				_baseImgPathTb.Text = "";
			}
		}

		#endregion

		#region Event handlers

		private void MainForm_Load(object sender, EventArgs e)
		{
			WindowState = FormWindowState.Maximized;

			_baseImgPath = Settings.Default.ImageBasePath;
			PbrtPath = Settings.Default.PBRTPath;

			foreach (var imageName in _imageNames)
			{
				CreateImageWindows(imageName.Key, imageName.Value);
			}

			UpdateBaseImgPath();

			CreateSceneEditor();
			
			_fnTb.Text = Settings.Default.FrameNumber.ToString();
		}

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			Settings.Default.ImageBasePath = _baseImgPath;
			Settings.Default.PBRTPath = _pbrtPath;
			Settings.Default.PbrtSceneFile = _editor.SceneFile;
			int fn;
			int.TryParse(_fnTb.Text, out fn);
			Settings.Default.FrameNumber = fn;
			Settings.Default.Save();
		}

		private void _baseImgPathTb_TextChanged(object sender, EventArgs e)
		{
			_baseImgPath = _baseImgPathTb.Text;
			UpdateBaseImgPath();
		}

		private void _pbrtPathTb_TextChanged(object sender, EventArgs e)
		{
			PbrtPath = _pbrtPathTb.Text;
		}

		private void _baseImgPathBtn_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog fd = new FolderBrowserDialog {SelectedPath = _baseImgPath};

			if (fd.ShowDialog() == DialogResult.OK)
			{
				_baseImgPath = fd.SelectedPath;
				UpdateBaseImgPath();
			}

		}

		private void _pbrtPathBtn_Click(object sender, EventArgs e)
		{
			OpenFileDialog fd = new OpenFileDialog
			{
				Filter = @"executable (*.exe)|*.exe",
				FileName = PbrtPath,
				Multiselect = false
			};

			if (fd.ShowDialog() == DialogResult.OK)
			{
				PbrtPath = fd.FileName;
			}
		}

		private void ChildForm_SizeChanged(object sender, EventArgs e)
		{
			var childForm = sender as DockableToolWindow;
			if (childForm != null && !_updatingSize.Contains(childForm))
			{
				_updatingSize.Add(childForm);
				childForm.Width = 400;
				childForm.Height = 300;
				_updatingSize.Remove(childForm);
			}
		}

		private void _run_Click(object sender, EventArgs e)
		{
			if (!File.Exists(_editor.SceneFile))
			{
				MessageBox.Show(@"Scene file does not exist. Select valid scene file and try again.", @"Scene file does not exist",
					MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return;
			}

			int fn;
			if (!int.TryParse(_fnTb.Text, out fn))
			{
				if (MessageBox.Show(
					    @"The provided frame number can not be parsed. Would you like to continue with rendering the default frame?",
					    @"Not a number",
					    MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
				{
					return;
				}

				fn = 0;
			}

			_editor.Save();
			string command = $"/C {PbrtPath} {_editor.SceneFile} --fn {fn}";
			var pi = new ProcessStartInfo("cmd.exe", command) { WorkingDirectory = _baseImgPath };
			var p = Process.Start(pi);
			p.WaitForExit();

			ReloadImages();
		}

		private void _fnTb_TextChanged(object sender, EventArgs e)
		{
			int fn;
			if (int.TryParse(_fnTb.Text, out fn))
			{
				_fnTb.ForeColor = Color.Black;
				_fnLbl.ForeColor = Color.Black;
			}
			else
			{
				_fnTb.ForeColor = Color.Red;
				_fnLbl.ForeColor = Color.Red;
			}
		}

		private void About_Click(object sender, EventArgs e)
		{
			MessageBox.Show(
				"This is a simple tool to edit a PBRT scene file and see the result of the differential  rendering run.\n(c) 2017 Jack Purvis and Florian Treier\n\nThis software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details.\n FreeImage is used under the GPL, version 3 license. ",
				@"About", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		#endregion
	}
}
