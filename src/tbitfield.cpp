// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len){
    if (len < 1) {
        throw "Incorrect_Len";
    }
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / sizeof(TELEM) * 8;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0) || (n >= BitLen)) {
        throw " WRONG INDEX";
    }
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM res = 1 << (sizeof(TELEM) * 8 - n - 1);
    return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw "WRONG INDEX";
    }
    TELEM BitMask = GetMemMask(n % (sizeof(TELEM) * 8));
    TELEM index = GetMemMask(n);
    pMem[index] |= BitMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw "WRONG INDEX";
    }
    TELEM BitMask = GetMemMask(n % (sizeof(TELEM) * 8));
    TELEM index = GetMemMask(n);
    pMem[index] &= ~BitMask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n > BitLen)) {
        throw "WRONG INDEX";
    }
    TELEM BitMask = GetMemMask(n % (sizeof(TELEM) * 8));
    TELEM index = GetMemMask(n);
    if ((pMem[index] & BitMask) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    if (BitLen != bf.BitLen) {
        throw "DIFFERENT SIZES";
    }
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int res = 1;
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            res = 0;
        }
        return res;
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int res = 0;
    if (BitLen != bf.BitLen) {
        return 1;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 1;
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int maxLength = std::max(BitLen, bf.BitLen);
    TBitField res(maxLength);

    // Перебираем общие элементы
    int minMemLen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    // Копируем оставшиеся элементы
    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
        int minLength = std::min(BitLen, bf.BitLen);
        TBitField res(minLength);

        // Выполняем побитовое "И" только на пересекающихся частях
        for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
            res.pMem[i] = pMem[i] & bf.pMem[i];
        }

        return res;
    
}

TBitField TBitField::operator~(void) // отрицание
{
        TBitField result(BitLen);

        for (int i = 0; i < MemLen; i++) {
            result.pMem[i] = ~pMem[i];
        }

        // Обнуляем биты, превышающие BitLen
        int extraBits = MemLen * sizeof(TELEM) * 8 - BitLen;
        if (extraBits > 0) {
            for (int i = BitLen; i < MemLen * sizeof(TELEM) * 8; i++) {
                TELEM index = i / (sizeof(TELEM) * 8);
                TELEM Bit = i % (sizeof(TELEM) * 8);

                result.pMem[index] &= ~GetMemMask(Bit);
            }
        }

        return result;
    }


// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
        char is;

        for (int i = 0; i < bf.BitLen; i++) {
            istr >> is;

            if (is == '0') bf.ClrBit(i);
            else if (is == '1') bf.SetBit(i);
            else throw "Incorrect symbol";
        }

        return istr;
    
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
        for (int i = 0; i < bf.BitLen; i++)
            ostr << bf.GetBit(i);

        return ostr;
    
}
