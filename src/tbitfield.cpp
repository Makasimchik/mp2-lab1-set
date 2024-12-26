// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
    {
        if (len < 1) {
            throw "incorrect len";
        }
        BitLen = len;
        MemLen = (len + sizeof(TELEM) * 8 - 1) / sizeof(TELEM) * 8;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }



    }

    TBitField::TBitField(const TBitField & bf) // конструктор копирования
    {
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        BitLen = bf.BitLen;
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
        return (n / (sizeof(TELEM) * 8));
    }

    TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
    {
        return TELEM(1) << (n % (sizeof(TELEM) * 8));
    }

    // доступ к битам битового поля

    int TBitField::GetLength(void) const // получить длину (к-во битов)
    {
        return BitLen;
    }

    void TBitField::SetBit(const int n) // установить бит
    {
        if ((n < 0) || (n >= BitLen)) {

            throw "incorrect";
        }
        else {
            pMem[GetMemIndex(n)] |= GetMemMask(n);
        }
    }


    void TBitField::ClrBit(const int n) // очистить бит
    {
        if (n < 0 || n >= BitLen) {
            throw "incorrect";
        }
        else {
            this->pMem[GetMemIndex(n)] &= ~GetMemMask(n);
        }

    }

    int TBitField::GetBit(const int n) const // получить значение бита
    {
        if (n < 0 || n >= BitLen) {
            throw "incorrect";
        }
        else {
            TELEM mask = GetMemMask(n);
            int index = GetMemIndex(n);
            if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }

    // битовые операции

    TBitField& TBitField::operator=(const TBitField & bf) // присваивание
    {
        if (this == &bf) {
            return *this;
        }

        delete[] pMem;

        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }

        return *this;
    }


    int TBitField::operator==(const TBitField & bf) const // сравнение
    {
        if (BitLen != bf.BitLen)
        {
            return 0;
        }
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                return 0;
            }
        }
        return 1;
    }


    int TBitField::operator!=(const TBitField & bf) const // сравнение
    {
        if (BitLen != bf.BitLen)
        {
            return 1;
        }
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                return 1;
            }
        }
        return 0;
    }

    TBitField TBitField::operator|(const TBitField & bf) // операция "или"
    {
        int newBitLen = std::max(BitLen, bf.BitLen);
        TBitField ress(newBitLen);
        int maxMemLen = std::max(MemLen, bf.MemLen);

        for (int i = 0; i < maxMemLen; i++) {
            ress.pMem[i] = (i < MemLen ? pMem[i] : 0) | (i < bf.MemLen ? bf.pMem[i] : 0);
        }
        return ress;
    }

    TBitField TBitField::operator&(const TBitField & bf) // операция "и"
    {
        TBitField ress(max(BitLen, bf.BitLen));
        for (int i = 0; i < MemLen; i++) {
            ress.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return ress;
    }

    TBitField TBitField::operator~(void) // отрицание
    {
        TBitField ress(BitLen);
        for (int i = 0; i < BitLen; i++)
        {
            if (this->GetBit(i) == 0)
            {
                ress.SetBit(i);
            }
        }
        return ress;
    }

    // ввод/вывод

    istream& operator>>(istream & istr, TBitField & bf) // ввод
    {
        for (int i = 0; i < bf.BitLen; i++)
        {
            int bit;
            istr >> bit;
            if (bit)
            {
                bf.SetBit(i);
            }
        }

        return istr;
    }

    ostream& operator<<(ostream & ostr, const TBitField & bf) // вывод
    {
        for (int i = 0; i < bf.BitLen; i++)
        {
            ostr << bf.GetBit(i);
        }

        return ostr;
    }
