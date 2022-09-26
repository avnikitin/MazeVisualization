#pragma once
#include <iostream>
#include <vector>
#include <map>
#using <System.Drawing.dll>

namespace MazeBuilder {

	float szX = 500;
	float szY = 500;
	float scope = 1;

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Text;
	using namespace System::Drawing::Printing;


	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			
			setlocale(LC_ALL, "Russian");
			FILE* f = freopen("maze.txt", "r", stdin);
			if (f == NULL) {
				exit(1);
			}
			std::cin >> szX >> szY;
			scope = 500. / std::max(szX, szY);
			szX *= scope;
			szY *= scope;
			this->ClientSize = System::Drawing::Size(szX, szY);
			auto g = this->CreateGraphics();
			this->Show();
			int N;
			std::cin >> N;
			std::vector<wchar_t> vertices(N);
			for (int i = 0; i < N; i++) {
				wchar_t c;
				std::wcin >> c;
				vertices[i] = c;
			}
			std::map < std::wstring, std::vector<std::pair<float, float>>> edges;
			std::map < std::wstring, std::pair <float, float>> coord;
			Pen^ pen = gcnew Pen(Color::Blue);
			SolidBrush^ Brush = gcnew SolidBrush(Color::SandyBrown);
			g->FillRectangle(Brush, 0., 0., szX, szY);
			for (int i = 0; i < N; i++) {
				wchar_t name;
				std::wcin >> name;
				int x, y;
				std::cin >> x >> y;
				bool entry, ext, feeder, fake_feeder;
				std::cin >> entry >> ext >> feeder >> fake_feeder;
				auto myFont = gcnew System::Drawing::Font("Times New Roman", 0.7 * scope);
				auto col = Color::White;
				if (feeder) {
					col = Color::Yellow;
				}
				else if (fake_feeder) {
					col = Color::MediumPurple;
				}
				else if (entry) {
					col = Color::Green;
				}
				else if (ext) {
					col = Color::LightBlue;
				}
				Brush->Color = col;
				g->FillEllipse(Brush, (x - 1) * scope, szY - (y + 1) * scope, 1.9 * scope, 1.9 * scope);
				pen->Color = Color::Black;
				g->DrawEllipse(pen, (x - 1) * scope, szY - (y + 1) * scope, 1.9 * scope, 1.9 * scope);
				String^ s = gcnew String("");
				s += name;
				Brush->Color = Color::Black;
				g->DrawString(s, myFont, Brush, (x - 0.5) * scope, szY - (y + 0.6) * scope);
				int m;
				std::cin >> m;
				for (int j = 0; j < m; j++) {
					wchar_t to;
					std::wcin >> to;
					s += to;
					int mid;
					std::wstring cur_edge = L"";
					cur_edge += name;
					coord[cur_edge] = { (x - 0.05) * scope, szY - y * scope - 0.05 * scope };
					cur_edge += to;
					std::cin >> mid;
					edges[cur_edge].clear();
					for (int k = 0; k < mid; k++) {
						int to_x, to_y;
						std::cin >> to_x >> to_y;
						edges[cur_edge].push_back({ (to_x - 0.05) * scope, szY - to_y * scope - 0.05 * scope });
					}
					if (mid == -1) {
						edges[cur_edge].push_back({ -999, -999 });
					}
				}
			}
			int W;
			std::cin >> W;
			for (int i = 0; i < W; i++) {
				int a, b, c, d;
				std::cin >> a >> b >> c >> d;
				SolidBrush^ grayBrush = gcnew SolidBrush(Color::LightSlateGray);
				g->FillRectangle(grayBrush, a * scope, szY - d * scope, (c - a) * scope, (d - b) * scope);
			}
			g->Save();
			auto graphPath = gcnew System::Drawing::Drawing2D::GraphicsPath();
			f = freopen("path.txt", "r", stdin);
			if (f == NULL) {
				exit(1);
			}
			wchar_t cur, nxt;
			if (!(std::wcin >> cur)) {
				return;
			} else if (!(std::wcin >> nxt)) {
				std::wstring fi = L"";
				fi += cur;
				graphPath->AddLine(coord[fi].first, coord[fi].second, coord[fi].first, coord[fi].second);
			}
			else do {
				std::wstring edge = L"";
				edge += cur;
				edge += nxt;
				if (edges.count(edge) == 0) {
					exit(1);
				}
				auto v = edges[edge];
				if (v.size() == 0 || (v.size() == 1 && v[0].first == -999)) {
					std::wstring fi = L"";
					fi += edge[0];
					std::wstring se = L"";
					se += edge[1];
					graphPath->AddLine(coord[fi].first, coord[fi].second, coord[se].first, coord[se].second);
				}
				else {
					std::wstring fi = L"";
					fi += cur;
					float cur_x = coord[fi].first;
					float cur_y = coord[fi].second;
					for (int i = 0; i < v.size(); i++) {
						graphPath->AddLine(cur_x, cur_y, v[i].first, v[i].second);
						cur_x = v[i].first;
						cur_y = v[i].second;
					}
					std::wstring se = L"";
					se += nxt;
					float to_x = coord[se].first;
					float to_y = coord[se].second;
					graphPath->AddLine(cur_x, cur_y, to_x, to_y);
				}
				cur = nxt;
			} while (std::wcin >> nxt);

			Pen^ redPen = gcnew Pen(Color::Red, 3);
			g->DrawPath(redPen, graphPath);

		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

protected:

	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 553);
			this->Name = L"MyForm";
			this->Text = L"Maze";
			this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Shown);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MyForm_Shown(System::Object^ sender, System::EventArgs^ e) {
		
	}
	};
}
