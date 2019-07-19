#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "source.h"
#include "picture_and_probability.h"
#include <msclr\marshal_cppstd.h>


using namespace cv;
using namespace std;

bool g = false;
vector <Picture_and_Probability> res;

namespace Nizhny800 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ Nizhny800
	/// </summary>
	public ref class Nizhny800 : public System::Windows::Forms::Form
	{
	public:
		Nizhny800(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~Nizhny800()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;

	private: System::Windows::Forms::Label^ label3;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->button1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(549, 35);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(166, 41);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Upload image";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Nizhny800::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(9, 111);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(300, 300);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"Picture files(*.jpg)|*.jpg|All files(*.*)|*.*";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 25.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(9, 34);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(184, 42);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Nizhny800";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(323, 111);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(244, 25);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Information for object";
			// 
			// label3
			// 
			this->label3->AutoEllipsis = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(324, 146);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(382, 265);
			this->label3->TabIndex = 5;
			this->label3->Click += gcnew System::EventHandler(this, &Nizhny800::label3_Click);
			// 
			// Nizhny800
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(726, 422);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Nizhny800";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

			Ptr<ORB> detector = ORB::create();
			generate_Databse(detector, res);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		System::String^ fname;// объ€вление строковой переменной
		openFileDialog1->ShowDialog();//открываем диалоговое окно
		fname = openFileDialog1->FileName;//используем переменную дл€ хранени€ имени выбранного файла

		string fname1 = msclr::interop::marshal_as<std::string>(fname);
		string name = showResult(fname1, res);
		cout << "name = " << name << "\n";
		string path_img1 = "C:/database/" + name + "/1.png";
		System::String^ path_img = gcnew System::String(path_img1.c_str());
		string path_txt1 = "C:/database/" + name + "/1.txt";
		System::String^ path_txt = gcnew System::String(path_txt1.c_str());
		label3->Text = System::IO::File::ReadAllText(path_txt, System::Text::Encoding::Default);
		pictureBox1->Image = Image::FromFile(path_img);//загружаем выбранный файл в элемент PictureBox
		//textBox1->Text = System::IO::File::ReadAllText(openFileDialog1->FileName,System::Text::Encoding::Default); //открытие выбранного текстового файла 
		//textBox1->Text = System::IO::File::ReadAllText("C:/Users/Admin/Desktop/nizhniy-800/klass1.txt", System::Text::Encoding::Default);//открытие указанного текстового файла 


		//Ptr<ORB> detector = ORB::create();
		//vector <Picture_and_Probability> res;
		//generate_Databse(detector, res);
		//showResult("../../nizhniy-800/database\\Alexander Nevsky Cathedral\\1.jpg", res);

	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
