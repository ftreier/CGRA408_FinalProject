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
			this._dockContainer = new Crom.Controls.DockContainer();
			this.SuspendLayout();
			// 
			// _dockContainer
			// 
			this._dockContainer.AutoSize = true;
			this._dockContainer.BackColor = System.Drawing.SystemColors.Window;
			this._dockContainer.BottomPanelHeight = 150;
			this._dockContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this._dockContainer.LeftPanelWidth = 150;
			this._dockContainer.Location = new System.Drawing.Point(0, 0);
			this._dockContainer.MinimumSize = new System.Drawing.Size(504, 528);
			this._dockContainer.Name = "_dockContainer";
			this._dockContainer.RightPanelWidth = 150;
			this._dockContainer.Size = new System.Drawing.Size(1200, 800);
			this._dockContainer.TabIndex = 0;
			this._dockContainer.TopPanelHeight = 150;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1200, 800);
			this.Controls.Add(this._dockContainer);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private DockContainer _dockContainer;
	}
}

