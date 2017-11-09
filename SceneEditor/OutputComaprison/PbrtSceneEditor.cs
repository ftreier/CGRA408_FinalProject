using System;
using System.IO;
using System.Windows.Forms;
using OutputComaprison.Properties;

namespace OutputComaprison
{
	public partial class PbrtSceneEditor : UserControl
	{
		private string _sceneFile;

		public string SceneFile
		{
			get { return _sceneFile; }
			private set
			{
				if (string.Compare(_sceneFile, value, StringComparison.CurrentCultureIgnoreCase) != 0)
				{
					_sceneFile = value;
					UpdateSceneFile();
					LoadSeene();
				}
			}
		}

		private void UpdateSceneFile()
		{
			_sfTb.Text = _sceneFile;
		}

		private void LoadSeene()
		{
			if (!File.Exists(SceneFile))
			{
				_sceneEditor.Text = "";
				return;
			}

			using (StreamReader sr = File.OpenText(SceneFile))
			{
				string text = sr.ReadToEnd();
				_sceneEditor.Text = text;
				sr.Close();
			}
		}

		public void Save()
		{
			if (!File.Exists(SceneFile))
			{
				return;
			}
			
			File.Delete(SceneFile);

			using (StreamWriter sw = File.AppendText(SceneFile))
			{
				sw.Write(_sceneEditor.Text);
				sw.Close();
			}
		}

		public PbrtSceneEditor()
		{
			InitializeComponent();
		}

		private void _sfBtn_Click(object sender, EventArgs e)
		{
			OpenFileDialog fd = new OpenFileDialog
			{
				Filter = @"PBRT Scene files (*.pbrt)|*.pbrt",
				FileName = SceneFile,
				Multiselect = false
			};

			if (fd.ShowDialog() == DialogResult.OK)
			{
				SceneFile = fd.FileName;    
			}
		}

		private void PbrtSceneEditor_Load(object sender, EventArgs e)
		{
			SceneFile = Settings.Default.PbrtSceneFile;
		}

		private void _sfTb_TextChanged(object sender, EventArgs e)
		{
			SceneFile = _sfTb.Text;
		}
	}
}
