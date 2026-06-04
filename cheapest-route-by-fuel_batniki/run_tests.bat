@echo off
chcp 1251 > nul
echo. > results.txt
del out*.dot 2>nul



echo. >> results.txt
echo 1. Корректный граф (прямой маршрут) >> results.txt
cheapest-route-by-fuel.exe test01_correct.dot test01_correct.txt out01.dot >> results.txt 2>&1
type out01.dot >> results.txt 2>nul

echo. >> results.txt
echo 2. Корректный граф (маршрут через 2 города) >> results.txt
cheapest-route-by-fuel.exe test02_correct_path.dot test02_correct_path.txt out02.dot >> results.txt 2>&1
type out02.dot >> results.txt 2>nul

echo. >> results.txt
echo 3. Нет маршрута >> results.txt
cheapest-route-by-fuel.exe test03_no_route.dot test03_no_route.txt out03.dot >> results.txt 2>&1
type out03.dot >> results.txt 2>nul

echo. >> results.txt
echo 4. Петля в графе >> results.txt
cheapest-route-by-fuel.exe test04_loop.dot test04_loop.txt out04.dot >> results.txt 2>&1
type out04.dot >> results.txt 2>nul

echo. >> results.txt
echo 5. Кратные рёбра >> results.txt
cheapest-route-by-fuel.exe test05_multiple_edges.dot test05_multiple_edges.txt out05.dot >> results.txt 2>&1
type out05.dot >> results.txt 2>nul

echo. >> results.txt
echo 6. Ориентированный граф >> results.txt
cheapest-route-by-fuel.exe test06_digraph.dot test06_digraph.txt out06.dot >> results.txt 2>&1
type out06.dot >> results.txt 2>nul

echo. >> results.txt
echo 7. Отсутствует цена на бензин >> results.txt
cheapest-route-by-fuel.exe test07_no_price.dot test07_no_price.txt out07.dot >> results.txt 2>&1
type out07.dot >> results.txt 2>nul

echo. >> results.txt
echo 8. Неверный формат цены (1 знак) >> results.txt
cheapest-route-by-fuel.exe test08_price_1digit.dot test08_price_1digit.txt out08.dot >> results.txt 2>&1
type out08.dot >> results.txt 2>nul

echo. >> results.txt
echo 9. Неверный формат цены (3 знака) >> results.txt
cheapest-route-by-fuel.exe test09_price_3digit.dot test09_price_3digit.txt out09.dot >> results.txt 2>&1
type out09.dot >> results.txt 2>nul

echo. >> results.txt
echo 10. Цена меньше 50 >> results.txt
cheapest-route-by-fuel.exe test10_price_less50.dot test10_price_less50.txt out10.dot >> results.txt 2>&1
type out10.dot >> results.txt 2>nul

echo. >> results.txt
echo 11. Цена больше 90 >> results.txt
cheapest-route-by-fuel.exe test11_price_more90.dot test11_price_more90.txt out11.dot >> results.txt 2>&1
type out11.dot >> results.txt 2>nul

echo. >> results.txt
echo 12. Заглавная буква в названии >> results.txt
cheapest-route-by-fuel.exe test12_uppercase_name.dot test12_uppercase_name.txt out12.dot >> results.txt 2>&1
type out12.dot >> results.txt 2>nul

echo. >> results.txt
echo 13. Название слишком короткое (2 символа) >> results.txt
cheapest-route-by-fuel.exe test13_name_too_short.dot test13_name_too_short.txt out13.dot >> results.txt 2>&1
type out13.dot >> results.txt 2>nul

echo. >> results.txt
echo 14. Название слишком длинное (55 символов) >> results.txt
cheapest-route-by-fuel.exe test14_name_too_long.dot test14_name_too_long.txt out14.dot >> results.txt 2>&1
type out14.dot >> results.txt 2>nul

echo. >> results.txt
echo 15. Запрещённые символы в названии (цифры) >> results.txt
cheapest-route-by-fuel.exe test15_bad_chars.dot test15_bad_chars.txt out15.dot >> results.txt 2>&1
type out15.dot >> results.txt 2>nul

echo. >> results.txt
echo 16. Дублирование названий городов >> results.txt
cheapest-route-by-fuel.exe test16_duplicate_name.dot test16_duplicate_name.txt out16.dot >> results.txt 2>&1
type out16.dot >> results.txt 2>nul

echo. >> results.txt
echo 17. Неожиданный атрибут shape >> results.txt
cheapest-route-by-fuel.exe test17_shape_attr.dot test17_shape_attr.txt out17.dot >> results.txt 2>&1
type out17.dot >> results.txt 2>nul

echo. >> results.txt
echo 18. Неверный формат label (нет ";") >> results.txt
cheapest-route-by-fuel.exe test18_no_semicolon.dot test18_no_semicolon.txt out18.dot >> results.txt 2>&1
type out18.dot >> results.txt 2>nul

echo. >> results.txt
echo 19. Неверный формат label (3 параметра) >> results.txt
cheapest-route-by-fuel.exe test19_3_params.dot test19_3_params.txt out19.dot >> results.txt 2>&1
type out19.dot >> results.txt 2>nul

echo. >> results.txt
echo 20. Меньше 2 вершин >> results.txt
cheapest-route-by-fuel.exe test20_1_vertex.dot test20_1_vertex.txt out20.dot >> results.txt 2>&1
type out20.dot >> results.txt 2>nul

echo. >> results.txt
echo 21. Отсутствует название города >> results.txt
cheapest-route-by-fuel.exe test21_no_name.dot test21_no_name.txt out21.dot >> results.txt 2>&1
type out21.dot >> results.txt 2>nul

echo. >> results.txt
echo 22. TXT: нет пробела между городами >> results.txt
cheapest-route-by-fuel.exe test22_txt_no_space.dot test22_txt_no_space.txt out22.dot >> results.txt 2>&1
type out22.dot >> results.txt 2>nul

echo. >> results.txt
echo 23. TXT: три города в файле >> results.txt
cheapest-route-by-fuel.exe test23_txt_three_cities.dot test23_txt_three_cities.txt out23.dot >> results.txt 2>&1
type out23.dot >> results.txt 2>nul

echo. >> results.txt
echo 24. TXT: заглавная буква >> results.txt
cheapest-route-by-fuel.exe test24_txt_uppercase.dot test24_txt_uppercase.txt out24.dot >> results.txt 2>&1
type out24.dot >> results.txt 2>nul

echo. >> results.txt
echo 25. TXT: название слишком короткое >> results.txt
cheapest-route-by-fuel.exe test25_txt_too_short.dot test25_txt_too_short.txt out25.dot >> results.txt 2>&1
type out25.dot >> results.txt 2>nul

echo. >> results.txt
echo 26. TXT: название слишком длинное >> results.txt
cheapest-route-by-fuel.exe test26_txt_too_long.dot test26_txt_too_long.txt out26.dot >> results.txt 2>&1
type out26.dot >> results.txt 2>nul

echo. >> results.txt
echo 27. TXT: цифры в названии >> results.txt
cheapest-route-by-fuel.exe test27_txt_digits.dot test27_txt_digits.txt out27.dot >> results.txt 2>&1
type out27.dot >> results.txt 2>nul

echo. >> results.txt
echo 28. TXT: начальный и конечный город совпадают >> results.txt
cheapest-route-by-fuel.exe test28_txt_same.dot test28_txt_same.txt out28.dot >> results.txt 2>&1
type out28.dot >> results.txt 2>nul

echo. >> results.txt
echo 29. TXT: пустая строка >> results.txt
cheapest-route-by-fuel.exe test29_txt_empty.dot test29_txt_empty.txt out29.dot >> results.txt 2>&1
type out29.dot >> results.txt 2>nul

echo. >> results.txt
echo 30. Комбинированный тест (несколько ошибок) >> results.txt
cheapest-route-by-fuel.exe test30_combined.dot test30_combined.txt out30.dot >> results.txt 2>&1
type out30.dot >> results.txt 2>nul



echo Done. See results.txt
exit