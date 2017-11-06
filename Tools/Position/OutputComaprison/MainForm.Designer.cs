using Crom.Controls;

namespace OutputComaprison
{
	partial class MainForm
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this._dockContainer = new Crom.Controls.DockContainer();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this._run = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this._pbrtPathLbl = new System.Windows.Forms.ToolStripLabel();
			this._pbrtPathTb = new System.Windows.Forms.ToolStripTextBox();
			this._pbrtPathBtn = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this._baseImgPathLbl = new System.Windows.Forms.ToolStripLabel();
			this._baseImgPathTb = new System.Windows.Forms.ToolStripTextBox();
			this._baseImgPathBtn = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this._fnLbl = new System.Windows.Forms.ToolStripLabel();
			this._fnTb = new System.Windows.Forms.ToolStripTextBox();
			this.tableLayoutPanel1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// _dockContainer
			// 
			this._dockContainer.AutoSize = true;
			this._dockContainer.BackColor = System.Drawing.SystemColors.Window;
			this._dockContainer.BottomPanelHeight = 150;
			this._dockContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this._dockContainer.LeftPanelWidth = 246;
			this._dockContainer.Location = new System.Drawing.Point(3, 44);
			this._dockContainer.MinimumSize = new System.Drawing.Size(604, 753);
			this._dockContainer.Name = "_dockContainer";
			this._dockContainer.RightPanelWidth = 154;
			this._dockContainer.Size = new System.Drawing.Size(1194, 753);
			this._dockContainer.TabIndex = 0;
			this._dockContainer.TopPanelHeight = 400;
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 1;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Controls.Add(this._dockContainer, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.toolStrip1, 0, 0);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 2;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(1200, 800);
			this.tableLayoutPanel1.TabIndex = 1;
			// 
			// toolStrip1
			// 
			this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._run,
            this.toolStripSeparator2,
            this._pbrtPathLbl,
            this._pbrtPathTb,
            this._pbrtPathBtn,
            this.toolStripSeparator1,
            this._baseImgPathLbl,
            this._baseImgPathTb,
            this._baseImgPathBtn,
            this.toolStripSeparator3,
            this._fnLbl,
            this._fnTb});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(1200, 41);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// _run
			// 
			this._run.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this._run.Enabled = false;
			this._run.Image = ((System.Drawing.Image)(resources.GetObject("_run.Image")));
			this._run.ImageTransparentColor = System.Drawing.Color.Magenta;
			this._run.Name = "_run";
			this._run.Size = new System.Drawing.Size(36, 38);
			this._run.Text = "Run";
			this._run.Click += new System.EventHandler(this._run_Click);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 41);
			// 
			// _pbrtPathLbl
			// 
			this._pbrtPathLbl.Name = "_pbrtPathLbl";
			this._pbrtPathLbl.Size = new System.Drawing.Size(121, 38);
			this._pbrtPathLbl.Text = "PBRT Path";
			// 
			// _pbrtPathTb
			// 
			this._pbrtPathTb.Name = "_pbrtPathTb";
			this._pbrtPathTb.Size = new System.Drawing.Size(300, 41);
			this._pbrtPathTb.TextChanged += new System.EventHandler(this._pbrtPathTb_TextChanged);
			// 
			// _pbrtPathBtn
			// 
			this._pbrtPathBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this._pbrtPathBtn.Image = ((System.Drawing.Image)(resources.GetObject("_pbrtPathBtn.Image")));
			this._pbrtPathBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this._pbrtPathBtn.Name = "_pbrtPathBtn";
			this._pbrtPathBtn.Size = new System.Drawing.Size(34, 38);
			this._pbrtPathBtn.Text = "...";
			this._pbrtPathBtn.Click += new System.EventHandler(this._pbrtPathBtn_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 41);
			// 
			// _baseImgPathLbl
			// 
			this._baseImgPathLbl.Name = "_baseImgPathLbl";
			this._baseImgPathLbl.Size = new System.Drawing.Size(190, 38);
			this._baseImgPathLbl.Text = "Base Image Path";
			// 
			// _baseImgPathTb
			// 
			this._baseImgPathTb.Name = "_baseImgPathTb";
			this._baseImgPathTb.Size = new System.Drawing.Size(300, 41);
			this._baseImgPathTb.TextChanged += new System.EventHandler(this._baseImgPathTb_TextChanged);
			// 
			// _baseImgPathBtn
			// 
			this._baseImgPathBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this._baseImgPathBtn.Image = ((System.Drawing.Image)(resources.GetObject("_baseImgPathBtn.Image")));
			this._baseImgPathBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this._baseImgPathBtn.Name = "_baseImgPathBtn";
			this._baseImgPathBtn.Size = new System.Drawing.Size(34, 38);
			this._baseImgPathBtn.Text = "...";
			this._baseImgPathBtn.Click += new System.EventHandler(this._baseImgPathBtn_Click);
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			this.toolStripSeparator3.Size = new System.Drawing.Size(6, 41);
			// 
			// _fnLbl
			// 
			this._fnLbl.Name = "_fnLbl";
			this._fnLbl.Size = new System.Drawing.Size(176, 32);
			this._fnLbl.Text = "Frame Number";
			// 
			// _fnTb
			// 
			this._fnTb.Name = "_fnTb";
			this._fnTb.Size = new System.Drawing.Size(100, 39);
			this._fnTb.TextChanged += new System.EventHandler(this._fnTb_TextChanged);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1200, 800);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Name = "MainForm";
			this.Text = "Form1";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private DockContainer _dockContainer;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripLabel _pbrtPathLbl;
		private System.Windows.Forms.ToolStripTextBox _pbrtPathTb;
		private System.Windows.Forms.ToolStripButton _pbrtPathBtn;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripLabel _baseImgPathLbl;
		private System.Windows.Forms.ToolStripTextBox _baseImgPathTb;
		private System.Windows.Forms.ToolStripButton _baseImgPathBtn;
		private System.Windows.Forms.ToolStripButton _run;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripLabel _fnLbl;
		private System.Windows.Forms.ToolStripTextBox _fnTb;
	}
}

