Funkcijų, kurios veikia taip pat vektorius klasėje ir std::vector pavyzdžiai:

push_back() - į vektorių įdeda paduotą reikšmę.
begin() - gražina vektoriaus pirmo elemento adresą.
insert() - įterpia duotą reikšmę į nurodytą poziciją.
size() - gražina skaičių, kuris rodo, kiek elementų yra vektoriuje.
at() - gražina elementą, esantį nurodytoje pozicijoje.

vektorius ir std::vector pildymas int tipo elementais:

|  | std::vector | vektorius |
| --- | --- | --- |
| 10000 |      0ms |            0ms |
| 100000 |    0ms |            0ms |
| 1000000 |   3ms |           1ms |
| 10000000 |  39ms |          27ms |
| 100000000 | 511ms |         406ms |

konteinerių (vektorius ir std::vector) atminties perskirstymų skaičius užpildant juos 100000000 elementų.

std::vector: 47

vektorius: 28

spartos analizė, naudojant duomenų failą su 100000 studentų įrašų:
| klasė | nuskaitymas | rūšiavimas | išvedimas | studentų skirstymas | visas laikas (su įvedimu)) |
| --- | --- | --- | --- | --- | --- |
| std::vector | 312ms | 135ms | 110ms | 18ms | 5192ms |
| vektorius | 297ms | 236ms | 117ms | 34ms | 6532ms |
