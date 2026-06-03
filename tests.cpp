#include "pch.h"
#include "CppUnitTest.h"
#include "../cheapest-route-by-fuel/header.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    // ============================================================================
    // Тесты для parseDot (парсинг DOT-файла) - 19 тестов
    // ============================================================================

    TEST_CLASS(ParseDotTests)
    {
    public:
        TEST_METHOD(Test01_CorrectGraph)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 -- 2",
                "1 -- 3",
                "2 -- 4",
                "3 -- 4",
                "4 -- 5",
                "1 [label = \"moscow; цена 63,72\"]",
                "2 [label = \"rostov; цена 73,23\"]",
                "3 [label = \"voronezh; цена 65,56\"]",
                "4 [label = \"ufa; цена 74,52\"]",
                "5 [label = \"samara; цена 56,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsTrue(result);
            Assert::AreEqual(5, g.getCityCount());
        }

        TEST_METHOD(Test02_OrientedGraph)
        {
            std::vector<std::string> lines = {
                "digraph G {",
                "1 -> 2",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test03_HasLoops)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 -- 1",
                "1 [label = \"moscow; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test04_MultipleEdges)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 -- 2",
                "1 -- 2",
                "1 [label = \"moscow; цена 63,72\"]",
                "2 [label = \"rostov; цена 73,23\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test05_VertexPriceMissing)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test06_VertexPriceFormatOneDigit)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 63,7\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test07_VertexPriceLessThan50)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 47,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test08_VertexPriceGreaterThan90)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 95,00\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test09_VertexNameUpperCase)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"Moscow; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test10_VertexNameTooShort)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"ab; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test11_VertexNameTooLong)
        {
            std::string longName = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz123";
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"" + longName + "; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test12_VertexNameBadChars)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow123; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test13_VertexNameDuplicate)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 63,72\"]",
                "2 [label = \"moscow; цена 73,23\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test14_VertexUnexpectedAttr)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 63,72\", shape=\"box\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test15_VertexLabelNoSemicolon)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test16_VertexLabelThreeParams)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 63,72; extra\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test17_LessThanTwoVertices)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"moscow; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test18_VertexNameMissing)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 [label = \"; цена 63,72\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test19_CombinedErrors)
        {
            std::vector<std::string> lines = {
                "graph G {",
                "1 -- 2",
                "1 -- 2",
                "1 [label = \"Moscow; цена 47,72\"]",
                "2 [label = \"rostov; цена 73,23\"]",
                "}"
            };
            Graph g;
            bool result = parseDot(lines, g);
            Assert::IsFalse(result);
            Assert::IsTrue(g.hasErrors());
        }
    };

    // ============================================================================
    // Тесты для parseTxt (парсинг TXT-файла) - 9 тестов
    // ============================================================================

    TEST_CLASS(ParseTxtTests)
    {
    public:
        TEST_METHOD(Test01_CorrectInput)
        {
            std::string content = "moscow samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsTrue(result);
            Assert::AreEqual("moscow", start.c_str());
            Assert::AreEqual("samara", end.c_str());
        }

        TEST_METHOD(Test02_NoSpace)
        {
            std::string content = "moscowsamara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test03_ThreeCities)
        {
            std::string content = "moscow rostov samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test04_Uppercase)
        {
            std::string content = "Moscow samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test05_NameTooShort)
        {
            std::string content = "ab samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test06_NameTooLong)
        {
            std::string longName = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz123";
            std::string content = longName + " samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test07_DigitsInName)
        {
            std::string content = "moscow123 samara";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test08_SameCities)
        {
            std::string content = "moscow moscow";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }

        TEST_METHOD(Test09_EmptyString)
        {
            std::string content = "";
            std::string start, end;
            std::set<Error> errors;
            bool result = parseTxt(content, start, end, errors);
            Assert::IsFalse(result);
            Assert::IsFalse(errors.empty());
        }
    };

    // ============================================================================
    // Тесты для addCity (добавление города) - 8 тестов
    // ============================================================================

    TEST_CLASS(AddCityTests)
    {
    public:
        TEST_METHOD(Test01_CorrectCity)
        {
            Graph g;
            g.addCity(1, "moscow", 63.72);
            Assert::AreEqual(1, g.getCityCount());
            Assert::AreEqual("moscow", g.cities[1].name.c_str());
        }

        TEST_METHOD(Test02_DuplicateName)
        {
            Graph g;
            g.addCity(1, "moscow", 63.72);
            g.addCity(2, "moscow", 73.23);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test03_EmptyName)
        {
            Graph g;
            g.addCity(1, "", 63.72);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test04_NameTooShort)
        {
            Graph g;
            g.addCity(1, "ab", 63.72);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test05_UppercaseName)
        {
            Graph g;
            g.addCity(1, "Moscow", 63.72);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test06_BadChars)
        {
            Graph g;
            g.addCity(1, "moscow123", 63.72);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test07_PriceLessThan50)
        {
            Graph g;
            g.addCity(1, "moscow", 47.72);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test08_PriceGreaterThan90)
        {
            Graph g;
            g.addCity(1, "moscow", 95.00);
            Assert::IsTrue(g.hasErrors());
        }
    };

    // ============================================================================
    // Тесты для addEdge (добавление ребра) - 3 теста
    // ============================================================================

    TEST_CLASS(AddEdgeTests)
    {
    public:
        TEST_METHOD(Test01_CorrectEdge)
        {
            Graph g;
            g.addCity(1, "moscow", 63.72);
            g.addCity(2, "rostov", 73.23);
            g.addEdge(1, 2);
            Assert::AreEqual((size_t)1, g.edges.size());
        }

        TEST_METHOD(Test02_Loop)
        {
            Graph g;
            g.addCity(1, "moscow", 63.72);
            g.addEdge(1, 1);
            Assert::IsTrue(g.hasErrors());
        }

        TEST_METHOD(Test03_MultipleEdges)
        {
            Graph g;
            g.addCity(1, "moscow", 63.72);
            g.addCity(2, "rostov", 73.23);
            g.addEdge(1, 2);
            g.addEdge(1, 2);
            Assert::IsTrue(g.hasErrors());
        }
    };

    // ============================================================================
    // Тесты для findPath (поиск маршрута) - 9 тестов
    // ============================================================================

    TEST_CLASS(FindPathTests)
    {
    public:
        TEST_METHOD(Test01_DirectRoute)
        {
            Graph g;
            g.addCity(1, "A", 63.72);
            g.addCity(2, "B", 73.23);
            g.addEdge(1, 2);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 2, path, cost);
            Assert::IsTrue(result);
            Assert::AreEqual(2, (int)path.size());
        }

        TEST_METHOD(Test02_PathThroughOneCity)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addCity(3, "C", 75);
            g.addEdge(1, 2);
            g.addEdge(2, 3);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 3, path, cost);
            Assert::IsTrue(result);
        }

        TEST_METHOD(Test03_ChooseCheapest)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addCity(3, "C", 60);
            g.addCity(4, "D", 0);
            g.addEdge(1, 2);
            g.addEdge(1, 3);
            g.addEdge(2, 4);
            g.addEdge(3, 4);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 4, path, cost);
            Assert::IsTrue(result);
        }

        TEST_METHOD(Test04_NoPath)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addCity(3, "C", 75);
            g.addCity(4, "D", 80);
            g.addEdge(1, 2);
            g.addEdge(3, 4);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 4, path, cost);
            Assert::IsFalse(result);
        }

        TEST_METHOD(Test05_StartEqualsEnd)
        {
            Graph g;
            g.addCity(1, "A", 50);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 1, path, cost);
            Assert::IsTrue(result);
            Assert::AreEqual(1, (int)path.size());
            Assert::AreEqual(0.0, cost);
        }

        TEST_METHOD(Test06_EqualCostPaths)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addCity(3, "C", 100);
            g.addCity(4, "D", 70);
            g.addEdge(1, 2);
            g.addEdge(1, 3);
            g.addEdge(2, 4);
            g.addEdge(3, 4);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 4, path, cost);
            Assert::IsTrue(result);
        }

        TEST_METHOD(Test07_AsymmetricDirect)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addEdge(1, 2);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 2, path, cost);
            Assert::IsTrue(result);
        }

        TEST_METHOD(Test08_AsymmetricReverse)
        {
            Graph g;
            g.addCity(1, "A", 50);
            g.addCity(2, "B", 100);
            g.addEdge(1, 2);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(2, 1, path, cost);
            Assert::IsTrue(result);
        }

        TEST_METHOD(Test09_ComplexGraph)
        {
            Graph g;
            g.addCity(1, "A", 63.72);
            g.addCity(2, "B", 73.23);
            g.addCity(3, "C", 65.56);
            g.addCity(4, "D", 74.52);
            g.addCity(5, "E", 56.72);
            g.addEdge(1, 2);
            g.addEdge(1, 3);
            g.addEdge(2, 4);
            g.addEdge(3, 4);
            g.addEdge(4, 5);
            std::vector<int> path;
            double cost;
            bool result = g.findPath(1, 5, path, cost);
            Assert::IsTrue(result);
        }
    };
}