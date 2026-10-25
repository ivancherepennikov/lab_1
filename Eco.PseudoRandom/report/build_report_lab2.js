/* Builds Report_Lab2_Aggregation.docx from data_lab2.json and project files. Run: NODE_PATH=<dir with docx> node build_report_lab2.js */
const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, Table, TableRow, TableCell, HeadingLevel, AlignmentType,
  LevelFormat, BorderStyle, WidthType, ShadingType, PageBreak, Footer, PageNumber,
} = require("docx");

const DEV = process.env.ECO_DEV || path.join(__dirname, "..", "..");
const data = JSON.parse(fs.readFileSync(path.join(__dirname, "data_lab2.json"), "utf8"));
const FONT = "Times New Roman";
const MONO = "Menlo";

function runs(text, base = {}) {
  const out = [];
  const re = /(\*\*[^*]+\*\*|`[^`]+`)/g;
  let last = 0, m;
  while ((m = re.exec(text)) !== null) {
    if (m.index > last) out.push(new TextRun({ text: text.slice(last, m.index), font: FONT, ...base }));
    const tok = m[0];
    if (tok.startsWith("**")) out.push(new TextRun({ text: tok.slice(2, -2), bold: true, font: FONT, ...base }));
    else out.push(new TextRun({ text: tok.slice(1, -1), font: MONO, size: (base.size || 24) - 2 }));
    last = m.index + tok.length;
  }
  if (last < text.length) out.push(new TextRun({ text: text.slice(last), font: FONT, ...base }));
  return out;
}
const P = (text, o = {}) => new Paragraph({ children: runs(text, o.run || {}), spacing: { after: o.after ?? 120, line: 300 }, alignment: o.align || AlignmentType.JUSTIFIED });
const H1 = (t, pb = false) => new Paragraph({ heading: HeadingLevel.HEADING_1, children: [new TextRun({ text: t, font: FONT })], spacing: { before: 360, after: 160 }, keepNext: true, pageBreakBefore: pb });
const H2 = (t) => new Paragraph({ heading: HeadingLevel.HEADING_2, children: [new TextRun({ text: t, font: FONT })], spacing: { before: 240, after: 120 }, keepNext: true });
const B = (t) => new Paragraph({ numbering: { reference: "bullets", level: 0 }, children: runs(t), spacing: { after: 80, line: 290 }, alignment: AlignmentType.LEFT });
const N = (t) => new Paragraph({ numbering: { reference: "numbers", level: 0 }, children: runs(t), spacing: { after: 80, line: 290 }, alignment: AlignmentType.LEFT });
const CAP = (t) => new Paragraph({ children: [new TextRun({ text: t, font: FONT, italics: true, size: 22 })], spacing: { before: 160, after: 80 }, keepNext: true, alignment: AlignmentType.LEFT });
const SP = () => new Paragraph({ children: [], spacing: { after: 120 } });
const border = { style: BorderStyle.SINGLE, size: 4, color: "808080" };
const borders = { top: border, bottom: border, left: border, right: border };
function cell(text, w, o = {}) {
  const lines = Array.isArray(text) ? text : [text];
  return new TableCell({
    width: { size: w, type: WidthType.DXA }, borders,
    shading: o.head ? { fill: "D9E2F3", type: ShadingType.CLEAR, color: "auto" } : (o.fill ? { fill: o.fill, type: ShadingType.CLEAR, color: "auto" } : undefined),
    margins: { top: 50, bottom: 50, left: 90, right: 90 },
    children: lines.map((l) => new Paragraph({
      alignment: AlignmentType.LEFT, spacing: { after: 0, line: 252 },
      children: o.head ? [new TextRun({ text: String(l), bold: true, font: FONT, size: o.size || 20 })]
        : (o.mono ? [new TextRun({ text: String(l), font: MONO, size: o.size || 16 })] : runs(String(l), { size: o.size || 20 })),
    })),
  });
}
function table(headers, rows, widths, o = {}) {
  const head = new TableRow({ tableHeader: true, children: headers.map((h, i) => cell(h, widths[i], { head: true, size: o.size })) });
  const body = rows.map((r) => new TableRow({ cantSplit: true, children: r.map((c, i) => cell(c, widths[i], { size: o.size, mono: o.monoCols && o.monoCols.includes(i) })) }));
  return new Table({ width: { size: widths.reduce((a, b) => a + b, 0), type: WidthType.DXA }, columnWidths: widths, rows: [head, ...body] });
}
function code(lines, size = 16) {
  return lines.map((l) => new Paragraph({
    shading: { fill: "F2F2F2", type: ShadingType.CLEAR, color: "auto" }, spacing: { after: 0, line: 240 }, alignment: AlignmentType.LEFT,
    children: [new TextRun({ text: l === "" ? " " : l, font: MONO, size })],
  }));
}

/* ---------- facts taken from the project files ---------- */
const rd = (p) => fs.readFileSync(path.join(DEV, p), "utf8").replace(/^﻿/, "").replace(/\r/g, "");
function cidOf(name) {
  const h = rd(`Eco.${name}/SharedFiles/IdEco${name}.h`).match(/GetIEcoComponentFactoryPtr_([0-9A-F]{32});/)[1];
  return [h.slice(0, 8), h.slice(8, 12), h.slice(12, 16), h.slice(16, 20), h.slice(20)].join("-");
}
function funcBody(file, startRe) {
  const s = rd(file);
  const m = s.match(startRe);
  if (!m) return ["(fragment not found)"];
  const a = m.index;
  const end = s.indexOf("\n}\n", a);
  return s.slice(a, end + 2).split("\n");
}
const t = data.tests;
const check = (n) => String(t[n].checks);

/* ---------- content ---------- */
const c = [];
c.push(new Paragraph({ children: [], spacing: { before: 1200 } }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 200 }, children: [new TextRun({ text: "Проектирование архитектуры и конструирование ПО", font: FONT, size: 28 })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 600 }, children: [new TextRun({ text: "Лабораторная работа № 2", font: FONT, size: 36, bold: true })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 200 }, children: [new TextRun({ text: "Повторное использование компонентов АОМК: включение и агрегирование", font: FONT, size: 30, bold: true })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 800 }, children: [new TextRun({ text: "Компоненты калькулятора A–E, внешние компоненты Eco.Lab1Outer и Eco.Lab2, генераторы лабораторной работы 1", font: FONT, size: 26 })] }));
for (const l of ["Студент: Черепенников Иван", "Группа: ТИДИ24", "Платформа: macOS (arm64), Xcode, Eco.Framework 20260918"]) {
  c.push(new Paragraph({ alignment: AlignmentType.RIGHT, spacing: { after: 80 }, children: [new TextRun({ text: l, font: FONT, size: 24 })] }));
}

c.push(H1("1. Постановка задачи", true));
c.push(P("Задание 2: в ранее созданный компонент с помощью приёмов программирования **включение** (containment) и **агрегирование** (aggregation) добавить компоненты калькулятора, выполняющие сложение, вычитание, умножение и деление. Клиент (юнит-тест) должен получать интерфейсы калькулятора вызовом `QueryInterface` и показать свойства интерфейсов: через любой указатель на интерфейс можно получить указатель на любой другой интерфейс компонента. Для высшей оценки внешний компонент должен поддерживать агрегирование, и должен быть реализован новый компонент Eco.Lab2, в котором включены и агрегированы все компоненты A, B, C, D, E и компонент лабораторной работы 1."));
c.push(H2("1.1. Принятые допущения"));
c.push(B("**Компоненты A–E.** В задании даны только схемы, определений нет. Принято (в соответствии с лекциями 2 и 4): A и B реализуют `IEcoCalculatorX` (сложение, вычитание); D и E – `IEcoCalculatorY` (умножение, деление); C – один компонент с обоими интерфейсами. B и E поддерживают агрегирование, A, C и D – только включение. IID и сигнатуры методов взяты из лекции 2."));
c.push(B("**Код лабораторной работы 1 не изменяется.** Три генератора (Fibonacci LFSR, Galois LFSR, Xorshift), клиент и тесты лабораторной работы 1 остались байт в байт теми же: в репозитории `git status` по их файлам показывает ноль изменений. Эти компоненты не поддерживают агрегирование (их `Create` игнорирует внешний `IEcoUnknown`), а менять их нельзя, поэтому генераторы **включаются** в новый внешний компонент."));
c.push(B("**«Eco.Lab1 с поддержкой агрегирования».** Роль компонента Eco.Lab1 из схем задания выполняет новый внешний компонент **Eco.Lab1Outer**: он предоставляет интерфейсы лабораторной работы 1 (`IEcoPseudoRandom1`, `IEcoPseudoRandomService1`), перенаправляя вызовы включённому генератору, содержит калькуляторы и сам поддерживает агрегирование."));
c.push(B("**Ограничение интерфейсов калькулятора.** Возвращаемые типы заданы лекцией (`Subtraction` и `Division` возвращают 16 бит, канала ошибки нет), поэтому переполнение и деление на ноль определяются политикой каждого компонента (см. таблицу 2)."));

c.push(H1("2. Архитектура"));
c.push(H2("2.1. Схема компонентов"));
c.push(...code([
  "Eco.Lab2                     IEcoLab2, IEcoCalculatorX, IEcoCalculatorY",
  "  |-- [aggregation] Eco.Lab1Outer     IEcoLab1Outer, IEcoPseudoRandom1, IEcoPseudoRandomService1",
  "  |       |-- [containment] Fibonacci LFSR | Galois LFSR | Xorshift      (lab 1, unchanged)",
  "  |       |-- X <- A (containment) | B (aggregation) | C (containment)",
  "  |       `-- Y <- D (containment) | E (aggregation) | C (containment)",
  "  |-- X <- A (containment) | B (aggregation) | C (containment)",
  "  `-- Y <- D (containment) | E (aggregation) | C (containment)",
], 17));
c.push(SP());
c.push(P("Схема объединяет рис. 1–3 задания. Внешний компонент Eco.Lab1Outer – схема рис. 1 и 2: интерфейс X предоставляют включённые A или C либо агрегированный B, интерфейс Y – включённые D или C либо агрегированный E. Eco.Lab2 – схема рис. 3: он агрегирует Eco.Lab1Outer (через него доступны интерфейсы лабораторной работы 1) и сам предоставляет X и Y от своих внутренних компонентов. Вариант внутреннего компонента выбирается методом `SelectCalculators(kindX, kindY)` собственного интерфейса внешнего компонента; по умолчанию используются A и D."));
c.push(H2("2.2. Компоненты"));
c.push(CAP("Таблица 1. Проекты и идентификаторы (каждый – отдельный проект eco-wizard со своей фабрикой и статической библиотекой)"));
c.push(table(["Проект", "Интерфейсы", "Агрегирование", "CID"], [
  ["Eco.CalcA", "IEcoCalculatorX", "нет (только включение)", cidOf("CalcA")],
  ["Eco.CalcB", "IEcoCalculatorX", "да", cidOf("CalcB")],
  ["Eco.CalcC", "IEcoCalculatorX, IEcoCalculatorY", "нет (только включение)", cidOf("CalcC")],
  ["Eco.CalcD", "IEcoCalculatorY", "нет (только включение)", cidOf("CalcD")],
  ["Eco.CalcE", "IEcoCalculatorY", "да", cidOf("CalcE")],
  ["Eco.Lab1Outer", "IEcoLab1Outer, X, Y, IEcoPseudoRandom1, IEcoPseudoRandomService1", "да", cidOf("Lab1Outer")],
  ["Eco.Lab2", "IEcoLab2, X, Y (+ все интерфейсы агрегированного Eco.Lab1Outer)", "да", cidOf("Lab2")],
], [1500, 3400, 1700, 3038], { size: 18, monoCols: [3] }));
c.push(SP());
c.push(CAP("Таблица 2. Интерфейсы калькулятора и политики компонентов (одинаковый контракт, различимое поведение)"));
c.push(table(["Компонент", "Subtraction(-32768, 1) при переполнении", "Subtraction(32767, -1)", "Division(1, 0)"], [
  ["A", "-32768 (насыщение)", "32767 (насыщение)", "–"],
  ["B", "32767 (циклический перенос)", "-32768 (циклический перенос)", "–"],
  ["C", "0", "0", "-1"],
  ["D", "–", "–", "0"],
  ["E", "–", "–", "32767 (насыщение)"],
], [1500, 3200, 2500, 2438], { size: 18 }));
c.push(SP());
c.push(P("`IEcoCalculatorX`: `int32_t Addition(a, b)`, `int16_t Subtraction(a, b)`; `IEcoCalculatorY`: `int32_t Multiplication(a, b)`, `int16_t Division(a, b)`; операнды 16-битные. Политики на выходе за пределы контракта намеренно разные: по ним тест и демонстрация определяют, какой именно внутренний компонент ответил, даже когда клиент держит только указатель на интерфейс."));

c.push(H1("3. Реализация механизмов"));
c.push(H2("3.1. Включение"));
c.push(P("Внешний компонент является клиентом внутреннего: при выборе варианта он запрашивает у шины внутренний компонент по CID и хранит указатель на его интерфейс; собственный интерфейс X (Y) реализован заново и переадресует вызовы внутреннему. Так подключены A, C и D, а в Eco.Lab1Outer – ещё и генератор лабораторной работы 1: интерфейсы `IEcoPseudoRandom1` и `IEcoPseudoRandomService1` перенаправляются выбранному генератору без каких-либо правок его кода."));
c.push(...code(funcBody("Eco.Lab1Outer/SourceFiles/CEcoLab1Outer.c", /static int32_t ECOCALLMETHOD CEcoLab1Outer_[0-9A-F]+_X_Addition\(/), 16));
c.push(SP());
c.push(H2("3.2. Агрегирование"));
c.push(P("Агрегируемый компонент (B, E) содержит **два** набора методов `IEcoUnknown`. *Делегирующие* (в таблице функций его интерфейса калькулятора) всегда передают вызов контролирующему `IEcoUnknown`: внешнему компоненту при агрегировании либо собственному неделегирующему, если компонент создан сам по себе. *Неделегирующий* `IEcoUnknown` ведёт настоящий счётчик ссылок и отвечает на `QueryInterface` за сам компонент; при запросе любого другого интерфейса он увеличивает счётчик через контролирующий `IEcoUnknown`, так что клиент и внешний компонент видят один счётчик."));
c.push(...code(funcBody("Eco.CalcB/SourceFiles/CEcoCalcB.c", /static int16_t ECOCALLMETHOD CEcoCalcB_[0-9A-F]+_Nondelegating_QueryInterface\(/), 16));
c.push(SP());
c.push(P("Внешний компонент создаёт агрегируемый внутренний вызовом `QueryComponent`, передавая свой `IEcoUnknown` третьим параметром и запрашивая именно `IID_IEcoUnknown`; полученный указатель – неделегирующий `IEcoUnknown` внутреннего компонента. В своём `QueryInterface` для интерфейса X (Y) внешний компонент передаёт запрос внутреннему и возвращает клиенту **его** указатель – вызовы идут напрямую, без переадресации:"));
c.push(...code([
  "result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalcB, pOuterUnk,",
  "                 &IID_IEcoUnknown, (void**)&pCMe->m_pXInner);          /* creation, aggregation */",
  "...",
  "if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {",
  "    if (pCMe->m_pXInner != 0) {                                        /* the pointer of B goes to the client */",
  "        return pCMe->m_pXInner->pVTbl->QueryInterface(pCMe->m_pXInner, riid, ppv);",
  "    }",
  "    ...                                                                /* otherwise this component's own interface */",
], 16));
c.push(SP());
c.push(P("Фабрика компонента, созданная визардом, была доработана в новых проектах: она обращается к объекту через его **неделегирующий** `IEcoUnknown` (иначе при создании с внешним компонентом запрос ушёл бы в ещё не готовый внешний), проверяет результат `Create` и освобождает память при отказе. Компоненты без поддержки агрегирования в `Create` возвращают `ERR_ECO_NOAGGREGATION`, если им передан внешний `IEcoUnknown`."));
c.push(H2("3.3. Внешний компонент, который сам агрегируется"));
c.push(P("Eco.Lab1Outer одновременно агрегирует (B, E) и сам может быть агрегирован (его создаёт Eco.Lab2). Поэтому все его пять интерфейсов делегируют `IEcoUnknown` контролирующему, а шестая таблица – неделегирующий `IEcoUnknown`. Агрегируемым внутренним компонентам он передаёт собственный делегирующий `IEcoUnknown`: цепочка делегирования проходит вверх до самого внешнего компонента, так что в любой точке схемы счётчик ссылок и тождество `IEcoUnknown` общие. Eco.Lab2 обрабатывает собственные интерфейсы и X, Y, а все остальные запросы передаёт агрегированному Eco.Lab1Outer; интерфейсы, полученные оттуда, при обращении к X или Y вновь возвращаются к Eco.Lab2."));
c.push(H2("3.4. Освобождение и счётчик ссылок"));
c.push(P("При обнулении счётчика внешний компонент сначала фиксирует его значение единицей (чтобы повторный `Release` во время разрушения не уничтожил объект второй раз), затем освобождает внутренние компоненты: включённые – обычным `Release`, агрегированные – `Release` их неделегирующего `IEcoUnknown` – и только потом освобождает собственную память. Тест проверяет, что после всех операций последний `Release` возвращает 0, то есть ни одна ссылка не потеряна ни при включении, ни при агрегировании, ни при вложенном агрегировании."));

c.push(H1("4. Тестирование"));
c.push(P("Для каждого проекта написан юнит-тест (консольный клиент, использующий libc для вывода; компоненты libc не используют). Тесты проходят на статических сборках через интерфейсную шину; запуск: `Eco.PseudoRandom/tests/run_unittest.sh Eco.<Проект>`."));
c.push(CAP("Таблица 3. Результаты юнит-тестов"));
c.push(table(["Проект", "Проверок", "Что проверяется"], [
  ["Eco.CalcA", check("CalcA"), "арифметика, политика, правила QueryInterface, отказ от внешнего IEcoUnknown (на уровне шины и фабрики)"],
  ["Eco.CalcB", check("CalcB"), "то же, агрегирование подставным внешним компонентом: делегирование QueryInterface/AddRef/Release, неделегирующий IEcoUnknown, разрушение при последнем Release"],
  ["Eco.CalcC", check("CalcC"), "оба интерфейса X и Y одного экземпляра, переходы X↔Y, отказ от внешнего IEcoUnknown"],
  ["Eco.CalcD", check("CalcD"), "как A для интерфейса Y"],
  ["Eco.CalcE", check("CalcE"), "как B для интерфейса Y"],
  ["Eco.Lab1Outer", check("Lab1Outer"), "9 вариантов калькуляторов (включение и агрегирование), матрица QueryInterface, генераторы лабораторной работы 1 через указатели калькуляторов, общий счётчик ссылок, баланс ссылок, агрегирование самого компонента"],
  ["Eco.Lab2", check("Lab2"), "то же для схемы рис. 3 с агрегированным Eco.Lab1Outer; интерфейсы лабораторной работы 1 доступны через указатель агрегированного B"],
], [1700, 1100, 6838], { size: 18 }));
c.push(SP());
c.push(P("Чувствительность тестов проверена намеренными поломками: если Eco.Lab2 перестаёт передавать неизвестные запросы агрегированному Eco.Lab1Outer, или агрегируемый B увеличивает собственный счётчик вместо контролирующего `IEcoUnknown`, соответствующие тесты падают, а после восстановления кода проходят."));
c.push(H2("4.1. Демонстрация клиента (Eco.Lab2)"));
c.push(P("Клиент запрашивает интерфейсы калькулятора вызовом `QueryInterface` у указателя внешнего компонента (как в задании) и вызывает методы через полученные указатели. Вывод клиента:"));
c.push(...code(data.demo_lab2.filter((l, i) => !(i === data.demo_lab2.length - 1 && l === "")), 13));
c.push(SP());
c.push(P("В таблице вариантов видно, какой внутренний компонент ответил (по политике, таблица 2): B даёт `(-32768)-1 = 32767`, C – `0` и `1/0 = -1`, E – `1/0 = 32767`. В матрице `ok` означает, что `QueryInterface` из указателя строки на интерфейс столбца успешен и возвращает всегда один и тот же указатель; последняя часть показывает, что генераторы лабораторной работы 1 вызываются через указатель калькулятора, агрегированного из B, при этом сами компоненты лабораторной работы 1 не менялись."));

c.push(H1("5. Анализ и выводы"));
c.push(H2("5.1. Сравнение включения и агрегирования"));
c.push(CAP("Таблица 4. Включение и агрегирование в выполненной работе"));
c.push(table(["Критерий", "Включение (A, C, D, генераторы)", "Агрегирование (B, E, Eco.Lab1Outer в Eco.Lab2)"], [
  ["Код внешнего компонента", "Свой интерфейс X (Y) реализован заново и переадресует вызовы; можно добавить код до и после вызова", "Интерфейс внутреннего возвращается клиенту как есть; переадресации нет, специализировать вызовы нельзя"],
  ["Требования к внутреннему компоненту", "Никаких: подойдёт любой компонент, в том числе не поддерживающий агрегирование (лаба 1)", "Должен поддерживать агрегирование: делегирующий и неделегирующий IEcoUnknown, обработку внешнего IEcoUnknown в Create"],
  ["Тождество и счётчик ссылок", "Внутренний – отдельный объект со своим IEcoUnknown и счётчиком", "Один IEcoUnknown и один счётчик на весь составной объект"],
  ["Накладные расходы", "Дополнительный вызов на каждый метод", "Нет, но каждый вызов IEcoUnknown проходит цепочку делегирования"],
  ["Замена внутреннего при работе", "Указатели клиента остаются действительными: они принадлежат внешнему компоненту", "Указатели прежнего внутреннего компонента становятся недействительными"],
], [2200, 3700, 3738], { size: 18 }));
c.push(SP());
c.push(H2("5.2. Находки и ошибки, которые пришлось исправить"));
c.push(N("Шаблонная фабрика визарда не проверяет результат `Create` и обращается к объекту через его первый (делегирующий) `IEcoUnknown`. Для агрегирования это ошибка: фабрика обязана использовать неделегирующий. Исправлено в фабриках новых проектов; фабрики лабораторной работы 1 не затрагивались."));
c.push(N("Интерфейсная шина превращает любой отказ фабрики в код `ERR_ECO_COMPONENT_NOTFOUND` (−23); `ERR_ECO_NOAGGREGATION` виден только при прямом вызове фабрики. Тесты проверяют оба уровня."));
c.push(N("Два вызова `QueryComponent` создают два разных экземпляра: тождество `IEcoUnknown` определено только внутри экземпляра. Первая версия теста получала X и Y из двух вызовов и ожидала общий `IEcoUnknown` – ошибка теста, исправлена получением второго интерфейса через `QueryInterface`."));
c.push(N("Тождество компонента определяется сравнением результатов `QueryInterface(IID_IEcoUnknown)`, а не приведением указателя интерфейса (правило лекции 3). Первая версия теста сравнивала результат с приведением указателя первого интерфейса и ошибочно падала."));
c.push(N("Лабораторная работа 1 не поддерживает агрегирование, а менять её нельзя; поэтому для требования «компонент Eco.Lab1 поддерживает агрегирование» понадобилась обёртка Eco.Lab1Outer, включающая неизменённые генераторы."));
c.push(H2("5.3. Выводы"));
c.push(B("Реализованы пять компонентов калькулятора A–E (два агрегируемых и три только для включения) и два внешних компонента, в которых показаны оба приёма и их вложение друг в друга (схемы рис. 1–3)."));
c.push(B("Через указатель калькулятора, в том числе агрегированного, `QueryInterface` возвращает интерфейсы лабораторной работы 1, а вызов генераторов даёт значения, совпадающие с эталоном лабораторной работы 1 (`000059C3`, `0000E270`, `00042021`), без изменения её кода."));
c.push(B("Свойства интерфейсов подтверждены: для каждой пары интерфейсов каждого внешнего компонента `QueryInterface` успешен и возвращает один и тот же указатель; все интерфейсы дают один `IEcoUnknown` и используют один счётчик ссылок; баланс ссылок сохраняется при включении, агрегировании и вложенном агрегировании."));
c.push(B("Агрегирование экономит код и сохраняет единое тождество, но требует поддержки от внутреннего компонента; включение универсально и позволяет специализировать вызовы, но дублирует интерфейс во внешнем компоненте."));

c.push(H1("6. Соответствие критериям оценивания"));
c.push(table(["Критерий задания", "Как выполнено"], [
  ["4–5: включение хотя бы одного из A–E", "Включение A, C, D (и генераторов лабораторной работы 1) в Eco.Lab1Outer и Eco.Lab2"],
  ["6–7: включение и агрегирование всех A–E, отчёт", "Все пять: A, C, D включены, B и E агрегированы, в обоих внешних компонентах; 9 вариантов в тестах и демонстрации"],
  ["8: без ошибок, полный отчёт с анализом и выводами", "Раздел 5; тесты проходят, мутации ловятся"],
  ["9–10: агрегирование внешним компонентом в Eco.Lab1, новый Eco.Lab2 со всеми A–E и Eco.Lab1", "Eco.Lab1Outer поддерживает агрегирование (тест на подставном внешнем компоненте); Eco.Lab2 агрегирует Eco.Lab1Outer и содержит A–E"],
], [3600, 6038], { size: 18 }));
c.push(SP());
c.push(H1("Приложение. Состав поставки и запуск", true));
c.push(table(["Что", "Где (репозиторий lab_1)"], [
  ["Компоненты A–E", "Eco.CalcA … Eco.CalcE (исходники, BuildFiles/Mac, проекты Xcode, юнит-тест)"],
  ["Внешние компоненты", "Eco.Lab1Outer, Eco.Lab2"],
  ["Интерфейсы калькулятора и IDL", "Eco.CalcInterfaces/SharedFiles (копии лежат в каждом проекте)"],
  ["Генераторы кода и тестов", "tools/gen_calc.py, gen_outer.py, gen_calc_tests.py, gen_outer_tests.py, link_projects.py"],
  ["Отчёт и сбор данных", "Eco.PseudoRandom/report/build_report_lab2.js, collect_lab2.py"],
  ["Лабораторная работа 1", "Eco.FibonacciLFSR1, Eco.GaloisLFSR1, Eco.Xorshift1, Eco.Lab1 – без изменений"],
], [3200, 6438], { size: 18 }));
c.push(SP());
c.push(...code([
  "export ECO_FRAMEWORK=$HOME/Dev/Eco.Framework",
  "Eco.PseudoRandom/tests/run_unittest.sh Eco.Lab2        # builds the library and the unit test and runs it",
  "Eco.PseudoRandom/tests/run_unittest.sh Eco.CalcB",
], 16));

const doc = new Document({
  creator: "Lab2", title: "Лабораторная работа 2. Включение и агрегирование (АОМК)",
  styles: {
    default: { document: { run: { font: FONT, size: 24 } } },
    paragraphStyles: [
      { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true, run: { size: 30, bold: true, font: FONT }, paragraph: { spacing: { before: 360, after: 160 }, outlineLevel: 0 } },
      { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true, run: { size: 26, bold: true, font: FONT }, paragraph: { spacing: { before: 240, after: 120 }, outlineLevel: 1 } },
    ],
  },
  numbering: { config: [
    { reference: "bullets", levels: [{ level: 0, format: LevelFormat.BULLET, text: "•", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
    { reference: "numbers", levels: [{ level: 0, format: LevelFormat.DECIMAL, text: "%1.", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
  ] },
  sections: [{
    properties: { page: { size: { width: 11906, height: 16838 }, margin: { top: 1134, right: 1134, bottom: 1134, left: 1134 } } },
    footers: { default: new Footer({ children: [new Paragraph({ alignment: AlignmentType.CENTER, children: [new TextRun({ children: [PageNumber.CURRENT], font: FONT, size: 20 })] })] }) },
    children: c,
  }],
});
Packer.toBuffer(doc).then((buf) => {
  const out = path.join(__dirname, "Report_Lab2_Aggregation.docx");
  fs.writeFileSync(out, buf);
  console.log("written", out, buf.length, "bytes");
});
