#include "macierz.h"



/*
	Autor: Mateusz Kojro
*/

macierz::macierz() {
	size_x_ = 2;
	size_y_ = 2;

	//uzycie takiego sposobu pomaga zaoszczedzic alokacji
	//wymaganych do utworzenia wskaznika na tablice dwuwymiarowa
	//poprawia uklad danych w pamieci dodatkowo pozwala porownywac x kopiowac tabele o wiele szybciej
	arr_ = new double[size_x_ * size_y_];
}

macierz::macierz(std::initializer_list<std::initializer_list<double>> init_list)
{

	// std::initializer_list to obiekt zdefiniowany w STL sluzacy do implementacji list inicjalizacyjnych 
	// przechowuje iterator po kolejnych elementach w zwiazku z tym zeby stworzyc macierz potrzebujemy zagniezdzonych 
	// list po jednej dla kazdego wiersza

	// ustalamy rozmiary na podstawie odpowiednich list
	size_y_ = init_list.size();
	size_x_ = init_list.begin()->size();

	// sprawdzamy czy kazdy wiersz ma ta sama dlugosc jak nie wyrzucamy wyjatek
	for (auto i : init_list) {
		if (i.size() != size_x_) { throw std::out_of_range("Initializer list need to be the same lenght"); }
	}

	// alokuje pamiec na podstawie rozmiarow
	arr_ = new double[init_list.size() * init_list.begin()->size()];

	// wypelniam ja elementami list inicjalizacyjncyh
	unsigned i = 0;
	for (auto outer : init_list) { 
		for (double inside : outer) {
			// linter pokazuje ostrzerzenie o mozliwym wyjsciu poza zakres tablicy ale chyba nie powinno sie nigdy zdarzyc
			arr_[i++] = inside; 
		}
	}

	std::clog << size_x_ << size_y_ << std::endl;
}

// what to do when its the same
macierz::macierz(const macierz& other) {
	if (&other == this) { return; }
	this->size_x_ = other.size_x_;
	this->size_y_ = other.size_y_;
	arr_ = new double[size_x_ * size_y_];
	memcpy(arr_, other.arr_, sizeof(double) * size_x_ * size_y_);
}

macierz::macierz(unsigned size_x, unsigned size_y) {
	size_x_ = size_x;
	size_y_ = size_y;
	arr_ = new double[size_x_ * size_y_];
}


unsigned macierz::calc_adress(unsigned x, unsigned y) {
	// Konweruje adres w postaci x,y na element tablicy 1 wymiarowej
	return y * this->size_x_ + x;
}

void macierz::set(unsigned x, unsigned y, double value) {
#ifdef CHECK_BOUNDS
	if (x >= size_x_ || y >= size_y_) throw std::out_of_range("You are trying to acces element out of bounds");
#endif // CHECK BOUNDS
	arr_[calc_adress(x, y)] = value;
}



double macierz::at(unsigned _x, unsigned _y) {
//Jako ze sprawdzanie czy adres nie wychodzi poza zakresy tabel zuzywa duzo zasobow
//(akumulje sie wiele wywolan tej funckji po 2 porownaniach w kazdym)
//ustawiamy je jako dodatkowa func domyslnie wylaczana
#ifdef CHECK_BOUNDS
	if (x >= size_x_ || y >= size_y_) throw std::out_of_range("You are trying to acces element out of bounds");
#endif // CHECK BOUNDS
	return arr_[calc_adress(_x, _y)];
}

void macierz::print() {
	for (unsigned y = 0; y < size_y_; y++) {
		for (unsigned x = 0; x < size_x_; x++) {
			std::clog << this->at(x,y) << " ";
		}
		std::clog << "\n";
	}

	std::clog << "\n";
}

bool macierz::fast_compare(const macierz& other) {

	// memcmp - to funkcja w standardzie C porownujaca dwa bloki pamieci zwraca 0 (int) jezeli sa takie same
	// lub pierwszy bajt ktorym sie roznia (>0 jezeli wiekszy w pierwszym bloku)
	// W zwiazku z tym nasz wynik explicitly rzucamy na bool x odwracamy jego wartosc (0(bool) == false)
	return !(bool)memcmp(other.arr_, this->arr_, sizeof(double) * size_x_ * size_y_);
}

bool macierz::operator==(const macierz& other) {

	// jezeli maja ten sam adres tzn ze sa tym samym obiektem - moge zwrocic wczesniej
	if (this == &other) { return true; }

	// jezeli rozmiary nie sa takie same to nie beda rowne - moge zwrocic wczesniej
	if (other.size_x_ != this->size_x_) { return false; }
	if (other.size_y_ != this->size_y_) { return false; }

	// jezeli ktorykolwiek element nie jest rowny nie sa rowne
	for (unsigned i = 0; i < size_x_ * size_y_; i++) {
		if (this->arr_[i] != other.arr_[i]) { return false; }
	}
	return true;
}

// analoginczie jak w operatorze == tylko wartosci sa zanegowane
bool macierz::operator!=(const macierz& other) {
	if (this == &other) { return false; }
	if (other.size_x_ != this->size_x_) { return true; }
	if (other.size_y_ != this->size_y_) { return true; }

	for (unsigned i = 0; i < size_x_ * size_y_; i++) {
		if (this->arr_[i] != other.arr_[i]) { return true; }
	}
	return true;
}

const macierz& macierz::operator=(const macierz& other) {
	// Sprawdzamy czy nie probujemy przypisac macierzy samej do siebie 
	if (&other == this) { return *this; }
	size_x_ = other.size_x_;
	size_y_ = other.size_y_;
	//memcpy - funkcja w standardzie C sluzaca do kopiowania bloku pamieci 
	//(jej implementacja ma zlozonosc znaczaco lepsza niz O(n) jaka osiagneli bysmy kopiujac element po elemencie)
	memcpy(arr_, other.arr_, sizeof(double) * size_x_ * size_y_);
	return *this;
}

#define CHECK_DIMENTIONS

macierz macierz::operator*(const macierz& other) {
	//Sytuacja jak w przpadku sprawdzania zakresow tablicy
#ifdef CHECK_DIMENTIONS
	if (this->size_y_ != other.size_x_) { throw bad_dimentions_; } // throw custom exception if wrong sizes
#endif // CHECK_DIMENTIONS
	macierz result(this->size_x_, other.size_y_);

	for (unsigned y = 0; y < result.size_x_; y++) {
		for (unsigned x = 0; x < result.size_y_; x++) {
			// Multiply the y of A by the column of B to at the y, column of product.
			result(y, x) = 0;
			for (unsigned i = 0; i < this->size_y_; i++) {
		
					result(y, x) += this->arr_[calc_adress(y, i)] * other.arr_[calc_adress(i, x)];
				
			}
		}
	}
	return result;
}

macierz macierz::operator+(const macierz& other) {
#ifdef CHECK_DIMENTIONS
	if (this->size_x_ != other.size_y_) { throw bad_dimentions_; } // throw custom exception if wrong sizes
#endif // CHECK_DIMENTIONS
	macierz result(size_x_, size_y_);
	for (unsigned i = 0; i < size_x_ * size_y_; i++) {
		result.arr_[i] = this->arr_[i] + other.arr_[i];
	}
	return result;
}

macierz macierz::operator-(const macierz& other) {
#ifdef CHECK_DIMENTIONS
	if (this->size_x_ != other.size_y_) { throw bad_dimentions_; } // throw custom exception if wrong sizes
#endif // CHECK_DIMENTIONS
	macierz result(size_x_, size_y_);
	for (unsigned i = 0; i < size_x_ * size_y_; i++) {
		result.arr_[i] = this->arr_[i] - other.arr_[i];
	}
	return result;
}

double& macierz::operator()(unsigned x, unsigned y) {
#ifdef CHECK_BOUNDS
	if (x >= size_x_ || y >= size_y_) throw std::out_of_range("You are trying to acces element out of bounds");
#endif // CHECK BOUNDS
	return arr_[calc_adress(x, y)];
}

// Aplikuje func na macierz
// np.
// A.apply_function([](double x) { return x * 2; });
//  Pomnozy kazdy element A przez 2
void macierz::apply_function(double(*func)(double))
{
	for (unsigned i = 0; i < size_x_ * size_y_; i++) {
		this->arr_[i] = func(this->arr_[i]);
	}
}

macierz::~macierz() {
	delete[] arr_;
	arr_ = nullptr;
}
