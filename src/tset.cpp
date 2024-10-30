// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{ }

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return TBitField(BitField);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) { // Проверка на самоприсваивание
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this; // Возвращаем неконстантную ссылку на текущий объект
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (this->BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !((*this) == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet res((BitField | s.BitField).GetLength());
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet ress(*this);

    ress.BitField.SetBit(Elem);

    return ress;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet ress(*this);

    ress.BitField.ClrBit(Elem);

    return ress;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet ress(*this);

    ress.BitField = BitField & s.BitField;
    ress.MaxPower = max(this->MaxPower, s.MaxPower);

    return ress;

}

TSet TSet::operator~(void) // дополнение
{
    TSet ress(*this);

    ress.BitField = ~BitField;

    return ress;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int i = 0;
    while ((i >= 0) && (i < s.MaxPower))
    {
        s.InsElem(i);
        istr >> i;
    }
    return istr; // TODO!!!
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.MaxPower; i++)
        if (s.BitField.GetBit(i)) {
            ostr << i;
        }
    return ostr;

}
