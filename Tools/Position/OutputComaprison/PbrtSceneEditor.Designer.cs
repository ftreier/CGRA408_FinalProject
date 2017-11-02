namespace OutputComaprison
{
	partial class PbrtSceneEditor
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PbrtSceneEditor));
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this._sfLbl = new System.Windows.Forms.ToolStripLabel();
			this._sfTb = new System.Windows.Forms.ToolStripTextBox();
			this._sceneEditor = new System.Windows.Forms.TextBox();
			this._sfBtn = new System.Windows.Forms.ToolStripButton();
			this.tableLayoutPanel1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 1;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Controls.Add(this.toolStrip1, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this._sceneEditor, 0, 1);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 2;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(600, 400);
			this.tableLayoutPanel1.TabIndex = 0;
			// 
			// toolStrip1
			// 
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._sfLbl,
            this._sfTb,
            this._sfBtn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(600, 39);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// _sfLbl
			// 
			this._sfLbl.Name = "_sfLbl";
			this._sfLbl.Size = new System.Drawing.Size(123, 36);
			this._sfLbl.Text = "Scene File";
			// 
			// _sfTb
			// 
			this._sfTb.Name = "_sfTb";
			this._sfTb.Size = new System.Drawing.Size(300, 39);
			this._sfTb.TextChanged += new System.EventHandler(this._sfTb_TextChanged);
			// 
			// _sceneEditor
			// 
			this._sceneEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this._sceneEditor.Location = new System.Drawing.Point(3, 42);
			this._sceneEditor.Multiline = true;
			this._sceneEditor.Name = "_sceneEditor";
			this._sceneEditor.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this._sceneEditor.Size = new System.Drawing.Size(594, 355);
			this._sceneEditor.TabIndex = 1;
			// 
			// _sfBtn
			// 
			this._sfBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this._sfBtn.Image = ((System.Drawing.Image)(resources.GetObject("_sfBtn.Image")));
			this._sfBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this._sfBtn.Name = "_sfBtn";
			this._sfBtn.Size = new System.Drawing.Size(34, 36);
			this._sfBtn.Text = "...";
			this._sfBtn.Click += new System.EventHandler(this._sfBtn_Click);
			// 
			// PbrtSceneEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tableLayoutPanel1);
			this.Name = "PbrtSceneEditor";
			this.Size = new System.Drawing.Size(600, 400);
			this.Load += new System.EventHandler(this.PbrtSceneEditor_Load);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripLabel _sfLbl;
		private System.Windows.Forms.ToolStripTextBox _sfTb;
		private System.Windows.Forms.ToolStripButton _sfBtn;
		private System.Windows.Forms.TextBox _sceneEditor;
	}
}
