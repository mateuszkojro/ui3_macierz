#pragma once
#ifndef _MACIERZ_H
#define _MACIERZ_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstring>
//#include <immintrin.h>
#include <initializer_list>
#include <algorithm>

/*
	Autor: Mateusz Kojro
	Opis: Klasa implementujaca macierz dowolnych rozmiarow, obsulgujaca nastepujace dzialania:

	Utworzono: 26.10.2020
	Zmiany:
		26.10 - podstawowy szkielet x implementacja dodawania x odejmowania
		28.10 - podstawowa implementacjia mnozenia, porownywania x kopiowania
		04.11 - poprawiam api (@operator())
		05.11 - optymyalizacja
		06.11 - dodaje komentarze
		08.11 - dodaje liste inicjalizacyjna
*/

class macierz
{
public:

	macierz();
	macierz(std::initializer_list<std::initializer_list<double>> init);
	macierz(const macierz& _other);
	macierz(unsigned _size_x, unsigned _size_y);
	~macierz();

	double at(unsigned _x, unsigned _y);
	void set(unsigned  _x, unsigned  _y, double _value);
	void print();
	bool fast_compare(const macierz& other);
	void apply_function(double (*func)(double));


	// Uzywam "const macierz&" w celu unikniecia niepotrzebnych kopii
	const macierz& operator=(const macierz& other);
	macierz operator*(const macierz& other);
	macierz operator+(const macierz& other);
	macierz operator-(const macierz& other);
	bool operator==(const macierz& other);
	bool operator!=(const macierz& other);
	double& operator()(unsigned x, unsigned y);


private:
	// wskaznik na tablice przechowujaca wartosci macierzy 
	double* arr_;
	unsigned size_x_, size_y_;
	unsigned calc_adress(unsigned _x, unsigned _y);

	class wrong_size_exception : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Matricies of those dimentions cannot be multiplied";
		}
	} bad_dimentions_;


};
#endif // !MACIERZ_H
