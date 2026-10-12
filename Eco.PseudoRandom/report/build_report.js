/* Builds Report_Lab1_PRNG.docx from data.json and project files. Run: NODE_PATH=<dir with docx> node build_report.js */
const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, Table, TableRow, TableCell, HeadingLevel, AlignmentType,
  LevelFormat, BorderStyle, WidthType, ShadingType, PageBreak, Footer, PageNumber, TableOfContents,
} = require("docx");

const HOME = process.env.HOME;
const DEV = process.env.ECO_DEV || path.join(__dirname, "..", "..");
const data = JSON.parse(fs.readFileSync(path.join(__dirname, "data.json"), "utf8"));

const FONT = "Times New Roman";
const MONO = "Menlo";
const CONTENT_W = 9638; // A4 with 2 cm side margins, DXA

/* ---------- helpers ---------- */
function runs(text, base = {}) {
  // **bold** and `code` inline markers
  const out = [];
  const re = /(\*\*[^*]+\*\*|`[^`]+`)/g;
  let last = 0, m;
  while ((m = re.exec(text)) !== null) {
    if (m.index > last) out.push(new TextRun({ text: text.slice(last, m.index), font: FONT, ...base }));
    const tok = m[0];
    if (tok.startsWith("**")) out.push(new TextRun({ text: tok.slice(2, -2), bold: true, font: FONT, ...base }));
    else out.push(new TextRun({ text: tok.slice(1, -1), font: MONO, size: (base.size || 24) - 2, ...{ color: base.color } }));
    last = m.index + tok.length;
  }
  if (last < text.length) out.push(new TextRun({ text: text.slice(last), font: FONT, ...base }));
  return out;
}
const P = (text, o = {}) => new Paragraph({
  children: runs(text, o.run || {}), spacing: { after: o.after ?? 120, line: 300 },
  alignment: o.align || AlignmentType.JUSTIFIED, indent: o.indent, keepNext: o.keepNext,
});
const H1 = (t, pb = false) => new Paragraph({ heading: HeadingLevel.HEADING_1, children: [new TextRun({ text: t, font: FONT })], spacing: { before: 360, after: 160 }, keepNext: true, pageBreakBefore: pb });
const H2 = (t) => new Paragraph({ heading: HeadingLevel.HEADING_2, children: [new TextRun({ text: t, font: FONT })], spacing: { before: 240, after: 120 }, keepNext: true });
const B = (text, level = 0) => new Paragraph({ numbering: { reference: "bullets", level }, children: runs(text), spacing: { after: 80, line: 290 }, alignment: AlignmentType.LEFT });
const N = (text) => new Paragraph({ numbering: { reference: "numbers", level: 0 }, children: runs(text), spacing: { after: 80, line: 290 }, alignment: AlignmentType.LEFT });
const CAP = (t) => new Paragraph({ children: [new TextRun({ text: t, font: FONT, italics: true, size: 22 })], spacing: { before: 160, after: 80 }, keepNext: true, alignment: AlignmentType.LEFT });
const SPACER = () => new Paragraph({ children: [], spacing: { after: 120 } });

const border = { style: BorderStyle.SINGLE, size: 4, color: "808080" };
const borders = { top: border, bottom: border, left: border, right: border };
function cell(text, w, o = {}) {
  const lines = Array.isArray(text) ? text : [text];
  return new TableCell({
    width: { size: w, type: WidthType.DXA }, borders,
    shading: o.head ? { fill: "D9E2F3", type: ShadingType.CLEAR, color: "auto" } : (o.fill ? { fill: o.fill, type: ShadingType.CLEAR, color: "auto" } : undefined),
    margins: { top: 50, bottom: 50, left: 90, right: 90 },
    children: lines.map((l) => new Paragraph({
      alignment: o.align || AlignmentType.LEFT, spacing: { after: 0, line: 252 },
      children: o.mono ? [new TextRun({ text: String(l), font: MONO, size: o.size || 16 })]
        : (o.head ? [new TextRun({ text: String(l), bold: true, font: FONT, size: o.size || 20 })] : runs(String(l), { size: o.size || 20 })),
    })),
  });
}
function table(headers, rows, widths, o = {}) {
  const total = widths.reduce((a, b) => a + b, 0);
  const head = new TableRow({ tableHeader: true, children: headers.map((h, i) => cell(h, widths[i], { head: true, size: o.size })) });
  const body = rows.map((r) => new TableRow({
    cantSplit: true,
    children: r.map((c, i) => cell(c, widths[i], { size: o.size, mono: o.monoCols && o.monoCols.includes(i), fill: o.fillFn ? o.fillFn(r, i) : undefined })),
  }));
  return new Table({ width: { size: total, type: WidthType.DXA }, columnWidths: widths, rows: [head, ...body] });
}
function code(lines, size = 17) {
  return lines.map((l) => new Paragraph({
    shading: { fill: "F2F2F2", type: ShadingType.CLEAR, color: "auto" },
    spacing: { after: 0, line: 240 }, alignment: AlignmentType.LEFT,
    children: [new TextRun({ text: l === "" ? " " : l, font: MONO, size })],
  }));
}
const hex = (v, bits) => v.toString(16).toUpperCase().padStart(bits === 32 ? 8 : 16, "0");

/* ---------- data ---------- */
function cidInfo(proj, short) {
  const t = fs.readFileSync(path.join(DEV, proj, "SharedFiles", "IdEco" + short + ".h"), "utf8").replace(/^\uFEFF/, "");
  const hexid = t.match(/GetIEcoComponentFactoryPtr_([0-9A-F]{32});/)[1];
  const uuid = [hexid.slice(0, 8), hexid.slice(8, 12), hexid.slice(12, 16), hexid.slice(16, 20), hexid.slice(20)].join("-");
  return { uuid, cls: "CEco" + short + "_" + hexid.slice(-8), suffix: hexid.slice(-8) };
}
const CID = { fib: cidInfo("Eco.FibonacciLFSR1", "FibonacciLFSR1"), gal: cidInfo("Eco.GaloisLFSR1", "GaloisLFSR1"), xor: cidInfo("Eco.Xorshift1", "Xorshift1") };
const sizes = data.sizes;
const rowsCmp = data.rows.map((r) => [
  r.algo, String(r.bits), "0x" + r.seed, r.mask !== "0" ? "0x" + r.mask : "по умолч.",
  r.ref.slice(0, 3),
  (r.soa_ok && r.ooa_ok && r.same) ? "да" : "НЕТ",
]);
const rowsStat = data.stats.map((s) => [
  s.algo, String(s.bits), s.ones.toFixed(5), s.low.toFixed(5), s.chi2.toFixed(1),
  s.ooa.toFixed(0), s.soa.toFixed(0),
]);

// excerpt of the step function of the final Galois component
const galoisSrc = fs.readFileSync(path.join(DEV, "Eco.GaloisLFSR1/SourceFiles/CEcoGaloisLFSR1.c"), "utf8").replace(/^﻿/, "");
const mCore = galoisSrc.match(/static int16_t Core\([\s\S]*?\n}\n/);
const coreLines = mCore ? mCore[0].trimEnd().split("\n") : ["(function not found)"];
const mDeg = galoisSrc.match(/static uint16_t Degree\([\s\S]*?\n}\n/);
const degLines = mDeg ? mDeg[0].trimEnd().split("\n") : [];
const promptFull = fs.readFileSync(path.join(DEV, "Eco.PseudoRandom/prompts/P9_ours.md"), "utf8").replace(/\r/g, "").split("\n");
const promptModule = promptFull;

const lf = data.stats.filter((x) => !x.algo.startsWith("Xorshift"));
const xs = data.stats.filter((x) => x.algo.startsWith("Xorshift"));
const rng = (a, k) => Math.min(...a.map((x) => x[k])).toFixed(0) + "–" + Math.max(...a.map((x) => x[k])).toFixed(0);
const slow = data.stats.map((x) => Math.round((1 - x.ooa / x.soa) * 100));
const slowMin = Math.max(0, Math.min(...slow)), slowMax = Math.max(...slow);

/* ---------- content ---------- */
const c = [];

// Title page
c.push(new Paragraph({ children: [], spacing: { before: 1200 } }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 200 }, children: [new TextRun({ text: "Проектирование архитектуры и конструирование ПО", font: FONT, size: 28 })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 600 }, children: [new TextRun({ text: "Лабораторная работа № 1", font: FONT, size: 36, bold: true })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 200 }, children: [new TextRun({ text: "Разработка компонентов по технологии АОМК (ACOM)", font: FONT, size: 30, bold: true })] }));
c.push(new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 800 }, children: [new TextRun({ text: "Раздел 3. Генераторы псевдослучайных чисел, задачи 38–40: LFSR в конфигурациях Фибоначчи и Галуа, Xorshift", font: FONT, size: 26 })] }));
c.push(new Paragraph({ alignment: AlignmentType.RIGHT, spacing: { after: 80 }, children: [new TextRun({ text: "Студент: ______________________________", font: FONT, size: 24 })] }));
c.push(new Paragraph({ alignment: AlignmentType.RIGHT, spacing: { after: 80 }, children: [new TextRun({ text: "Группа: _______________________________", font: FONT, size: 24 })] }));
c.push(new Paragraph({ alignment: AlignmentType.RIGHT, spacing: { after: 80 }, children: [new TextRun({ text: "Платформа: macOS (arm64), Xcode, Eco.Framework 20260918", font: FONT, size: 24 })] }));

c.push(new Paragraph({ pageBreakBefore: true, children: [new TextRun({ text: "Содержание", font: FONT, size: 30, bold: true })], spacing: { after: 160 } }));
["1. Постановка задачи", "2. Архитектура решения", "3. Алгоритмы", "4. Реализация и сборка",
  "5. Тестирование и сравнение с эталоном", "6. Статистические проверки и скорость", "7. Сравнение подходов SOA и ООА",
  "8. Инжиниринг промтов и автоматизация разработки", "9. Выводы", "Приложение А. Полный текст финального системного промта (P9)",
  "Приложение Б. Состав поставки"].forEach((t) => c.push(new Paragraph({ children: [new TextRun({ text: t, font: FONT, size: 24 })], spacing: { after: 100 } })));

// 1
c.push(H1("1. Постановка задачи", true));
c.push(P("По списку заданий необходимо разработать программный компонент в рамках компонентной архитектуры АОМК (ACOM), расширяющий математическую функциональность системы, и реализовать для него интерфейс с методами согласно выбранной задаче. Выбран раздел 3 «Генераторы псевдослучайных чисел»:"));
c.push(B("**задача 38** – регистр сдвига с линейной обратной связью LFSR-k в конфигурации Фибоначчи;"));
c.push(B("**задача 39** – LFSR-k в конфигурации Галуа;"));
c.push(B("**задача 40** – генератор Xorshift."));
c.push(P("Критерий на 6–7 баллов для задач 38–40: реализация всех трёх алгоритмов в виде независимых компонентов с возможностью динамического переключения генератора в клиенте. Для раздела 3 доступны дополнительные баллы за архитектурное усложнение (ООА против SOA) и за инжиниринг системных промтов. Оба направления выполнены и описаны в разделах 7 и 8."));
c.push(P("Ограничения задания: запрещено использовать функции стандартной библиотеки libc и сторонние библиотеки в компонентах; работоспособность подтверждается юнит-тестами; клиент – консольное приложение."));

// 2
c.push(H1("2. Архитектура решения"));
c.push(H2("2.1. Общий контракт интерфейсов"));
c.push(P("Все три алгоритма реализуют один и тот же набор из двух интерфейсов. Благодаря этому клиент переключает алгоритм заменой одного идентификатора CID, а код работы с генератором остаётся неизменным (свойство взаимозаменяемости компонентов из лекции 2). Интерфейсы АОМК после публикации менять нельзя, поэтому контракт зафиксирован в комментариях заголовков и в файлах IDL."));
c.push(CAP("Таблица 1. Интерфейсы компонентов"));
c.push(table(["Интерфейс", "Метод", "Назначение"], [
  ["IEcoPseudoRandomService1\n(IID C13EFEC0-379D-4374-9B7F-F4E3815D6C31)\nбез состояния, SOA", "Step32(state, param, &newState)\nStep64(state, param, &newState)", "Один шаг генератора. Текущее состояние передаёт клиент и получает новое. Компонент между вызовами ничего не помнит."],
  ["IEcoPseudoRandom1\n(IID 36BD4863-DE93-40D6-B1FB-319B64A37722)\nс состоянием, ООА", "Init(seed, param, bits)\nNext(&value)\nget_Name()", "Объект хранит состояние регистра и полином. Init задаёт начальное состояние и разрядность (32 или 64), Next возвращает следующее значение, get_Name – название алгоритма."],
].map((r) => [r[0].split("\n"), r[1].split("\n"), r[2]]), [3300, 2900, 3438]));
c.push(SPACER());
c.push(P("**Общая семантика.** Параметр `param` для LFSR – маска отводов обратной связи, бит i соответствует члену x^(i+1) полинома. Разрядность регистра определяется самой маской: n = (номер старшего установленного бита) + 1. Значение `param = 0` выбирает полином по умолчанию с максимальным периодом: 0x80200003 для 32 бит (x^32+x^22+x^2+x+1) и 0xD800000000000000 для 64 бит. У Xorshift параметр `param` игнорируется. Состояние сначала усекается до n бит; нулевое состояние является неподвижной точкой всех алгоритмов и отвергается кодом `ERR_ECO_INVALIDARG`. Возвращаемое значение генератора равно новому состоянию регистра."));
c.push(P("**Коды возврата:** `ERR_ECO_SUCCESS`, `ERR_ECO_POINTER` (любой указатель или `me` равен NULL), `ERR_ECO_INVALIDARG` (нулевое состояние, разрядность не 32 и не 64), `ERR_ECO_FAIL` (вызов `Next` до `Init`)."));

c.push(H2("2.2. Три независимых компонента"));
c.push(CAP("Таблица 2. Компоненты (каждый – отдельный проект, своя фабрика и своя статическая библиотека)"));
c.push(table(["Задача", "Проект", "Класс реализации", "CID"], [
  ["38", "Eco.FibonacciLFSR1", CID.fib.cls, CID.fib.uuid],
  ["39", "Eco.GaloisLFSR1", CID.gal.cls, CID.gal.uuid],
  ["40", "Eco.Xorshift1", CID.xor.cls, CID.xor.uuid],
], [900, 2300, 3000, 3438]));
c.push(SPACER());

c.push(H2("2.3. SOA и ООА в одном экземпляре компонента"));
c.push(P("Каждый компонент поддерживает оба интерфейса одним объектом с единым счётчиком ссылок. В начале структуры объекта подряд лежат два указателя на таблицы виртуальных функций. Метод интерфейса 0 получает в параметре `me` адрес объекта, метод интерфейса 1 – адрес второго поля-таблицы, поэтому во всех методах второго интерфейса (включая QueryInterface, AddRef и Release) выполняется поправка `pCMe = (CX*)((char_t*)me - sizeof(voidptr_t))`. Метод QueryInterface каждого интерфейса знает все три идентификатора (Service, Object, IEcoUnknown), так что из любого интерфейса можно получить любой другой, как требуют правила лекции 3. Количество ссылок увеличивается при каждом успешном запросе интерфейса и уменьшается в Release; при нуле объект освобождается через аллокатор Eco.MemoryManager1."));

c.push(H2("2.4. Клиент «Калькулятор»"));
c.push(P("Консольный клиент (проект Eco.Lab1, файл `UnitTestFiles/SourceFiles/EcoLab1.c`) регистрирует три статических компонента на шине интерфейсов, запрашивает компонент по CID и работает с ним через оба интерфейса. Он выводит демонстрацию (для каждого алгоритма и каждой из двух разрядностей последовательности SOA и ООА и признак их совпадения), затем запускает интерактивное меню: выбор алгоритма, режима SOA или ООА, разрядности, начального состояния, маски полинома и количества значений. Пункт «9» печатает статистику качества и скорость."));
c.push(P("**Использование libc в клиенте.** На платформе macOS в составе Eco.Framework нет собранных для неё компонентов консольного ввода-вывода (Eco.StdIO.C89 и Eco.Log1 поставляются только для Linux и Windows). Поэтому клиент выводит данные функциями `printf` и `scanf` стандартной библиотеки. Это относится только к клиенту; сами компоненты libc не используют."));

// 3
c.push(H1("3. Алгоритмы"));
c.push(CAP("Таблица 3. Шаг генератора (state – состояние, m – маска разрядности n, p – маска полинома)"));
c.push(table(["Алгоритм", "Шаг", "Особенности"], [
  ["LFSR, конфигурация Фибоначчи (задача 38)", ["fb = parity(state & p)", "new = ((state << 1) | fb) & m"], "Сдвиг влево, новый бит – чётность отводов. Для 16-битной маски 0xB400 период равен 65535."],
  ["LFSR, конфигурация Галуа (задача 39)", ["lsb = state & 1; s = state >> 1", "если lsb, то s ^= p; new = s & m"], "Сдвиг вправо, обратная связь накладывается на несколько разрядов сразу. Один и тот же полином даёт тот же период, что у Фибоначчи."],
  ["Xorshift (задача 40), 32 бит", ["x ^= x << 13; x ^= x >> 17; x ^= x << 5"], "Все операции в 32-битном регистре. Полный период 2^32 − 1."],
  ["Xorshift (задача 40), 64 бит", ["x ^= x << 13; x ^= x >> 7; x ^= x << 17"], "Все операции в 64-битном регистре. Полный период 2^64 − 1."],
], [2600, 3600, 3438]));
c.push(SPACER());
c.push(P("Ключевой практический нюанс 32-битного режима: состояние хранится в 64-битном слове, поэтому результат каждого сдвига влево нужно усекать маской разрядности сразу, иначе старшие биты попадут в последующий сдвиг вправо и последовательность разойдётся с эталоном. Для LFSR разрядность определяется степенью полинома, а не разрядностью вызова: например, при `Step32(state = 0x10000, param = 0xB400)` регистр 16-битный, состояние усекается до нуля и вызов возвращает `ERR_ECO_INVALIDARG`."));

// 4
c.push(H1("4. Реализация и сборка"));
c.push(B("Язык C в строгом стандарте C89: комментарии только `/* */`, переменные объявляются в начале блока, 64-битные константы собираются из 32-битных частей (без суффиксов ULL)."));
c.push(B("Только типы Eco (`int16_t`, `uint32_t`, `uint64_t`, `char_t`, `voidptr_t`) из Eco.Core1; запрет libc, включая неявные `memcpy` и `memset`: память выделяется только через `IEcoMemoryAllocator1`."));
c.push(B("Каждый метод начинается с проверки указателей на NULL; крайние случаи (нулевое состояние, недопустимая разрядность, вызов Next до Init) возвращают коды `ERR_ECO_*`."));
c.push(B("Глобальные символы, видимые линковщику, содержат суффикс CID (например, `g_xIEcoPseudoRandom1VTbl_" + CID.gal.suffix + "`), остальное объявлено `static`: иначе при статической линковке нескольких компонентов в один бинарник одноимённые таблицы подменяют друг друга (см. раздел 8.3)."));
c.push(P("**Структура проекта компонента** создаётся утилитой eco-wizard (тип COM): `SharedFiles` (интерфейсы и идентификаторы), `HeaderFiles` и `SourceFiles` (реализация и фабрика), `UnitTestFiles` (тесты), `AssemblyFiles/Mac/Xcode_v123` (проекты Xcode)."));
c.push(P("**Сборка.** Из терминала для каждого компонента (Debug и Release), затем для клиента:"));
c.push(...code([
  "export ECO_FRAMEWORK=$HOME/Dev/Eco.Framework",
  "cd ~/Dev/Eco.GaloisLFSR1/AssemblyFiles/Mac/Xcode_v123",
  "xcodebuild -project EcoGaloisLFSR1.xcodeproj -scheme Static -configuration Release build",
  "cd ~/Dev/Eco.Lab1/AssemblyFiles/Mac/Xcode_v123",
  "xcodebuild -project EcoLab1UnitTest.xcodeproj -scheme Static -configuration Release \\",
  "           -derivedDataPath ../../../.dd_client build",
  "~/Dev/Eco.Lab1/.dd_client/Build/Products/Release/Static",
]));
c.push(SPACER());
c.push(P("В Xcode для сборки из среды необходимо задать пользовательский путь `ECO_FRAMEWORK` (Settings → Locations → Custom Paths)."));

// 5
c.push(H1("5. Тестирование и сравнение с эталоном"));
c.push(H2("5.1. Эталонная модель"));
c.push(P("В стандартной библиотеке C нет генераторов, которые можно использовать как независимый эталон для LFSR и Xorshift (функция `rand` реализует другой алгоритм), а MATLAB не входит в среду выполнения работы. Поэтому в качестве эталона написана независимая модель на Python (`reference/reference.py`), реализующая алгоритмы строго по описанию контракта. Модель проверена по опубликованным значениям Дж. Марсальи: Xorshift32 при начальном состоянии 1 даёт 270369, 67634689, 2647435461 (в шестнадцатеричном виде 00042021, 04080601, 9DCCA8C5), и клиент выводит ровно эти значения. Периоды 16-битных регистров для обеих конфигураций LFSR проверены перебором: 65535."));
c.push(H2("5.2. Контрактные юнит-тесты"));
c.push(P("Один и тот же набор тестов (`tests/PseudoRandomTests.h`) выполняется для каждого компонента; ожидаемые значения генерируются эталонной моделью в заголовок `PRNGVectors.h`. Тестовый клиент возвращает 0 при успехе и печатает секцию и номер первой упавшей проверки при неудаче. Набор проверяет:"));
c.push(B("значения 10 шагов для трёх начальных состояний при 32 и 64 битах (в том числе с 16-битной маской), через интерфейс SOA и через интерфейс ООА;"));
c.push(B("ошибочные аргументы: нулевое состояние, нулевое состояние в пределах 16-битного регистра, разрядность 0 и 16, нулевые указатели (`me` и выходные параметры), вызов Next до Init, повторный Init;"));
c.push(B("для LFSR – полный цикл периода 65535 для 16-битной маски;"));
c.push(B("правила QueryInterface из лекции 3: один и тот же IEcoUnknown через оба интерфейса, возврат к исходному интерфейсу, повторный запрос уже имеющегося интерфейса, отказ для неизвестного интерфейса, согласованность AddRef и Release;"));
c.push(B("корректное завершение (освобождение объектов): ошибки учёта ссылок приводили к аварийному завершению программы и фиксировались."));
c.push(P(`**Результат:** Fibonacci LFSR – 65816 проверок, Galois LFSR – 65816 проверок, Xorshift – 279 проверок (у LFSR больше за счёт цикла периода); все пройдены. Чувствительность набора проверена мутацией: замена маски по умолчанию в готовом компоненте приводит к падению теста.`));
c.push(H2("5.3. Сравнительная таблица значений"));
c.push(P("Значения, выданные компонентами через клиент (отдельно через SOA и через ООА), сравнивались с эталонной моделью для 14 комбинаций алгоритма, разрядности, начального состояния и маски. Для каждой комбинации сравнивались первые пять значений; в таблице показаны первые три."));
c.push(CAP("Таблица 4. Компонент и эталон (значения в шестнадцатеричном виде)"));
c.push(table(["Алгоритм", "Бит", "Seed", "Маска", "Эталон (первые 3 значения)", "SOA = ООА = эталон"], rowsCmp, [1500, 520, 1250, 1050, 4318, 1000], { size: 16, monoCols: [4] }));
c.push(SPACER());
const mism = data.rows.filter((r) => !(r.soa_ok && r.ooa_ok && r.same)).length;
c.push(P(`**Расхождений с эталоном: ${mism} из ${data.rows.length}.** Точность сравнения – побитовая (точное совпадение целых значений), а не до N десятичных знаков, поскольку генераторы целочисленные.`));
c.push(H2("5.4. Проверка при совместной линковке"));
c.push(P("Три компонента линкуются в один бинарник клиента. Проверка уникальности экспортируемых символов (`nm -g`) показывает отсутствие конфликтов между библиотеками; клиент загружает каждый компонент по его CID, и `get_Name` каждого загруженного компонента возвращает название своего алгоритма («Fibonacci LFSR», «GaloisLFSR1», «Xorshift (Marsaglia)»)."));

// 6
c.push(H1("6. Статистические проверки и скорость"));
c.push(P("Для каждого алгоритма и разрядности сгенерировано 1 000 000 значений в режиме ООА (начальное состояние 0x2545F491, полином по умолчанию). Определялись: доля единичных бит (идеал 0,5), доля единиц в младшем бите (классический выходной бит LFSR), статистика хи-квадрат по 256 значениям байта (идеал около 255; для 255 степеней свободы граница 1 % составляет примерно 310) и скорость в миллионах значений в секунду (macOS arm64, Release, замер `clock()` чистым циклом вызовов; отдельно для SOA и ООА)."));
c.push(CAP("Таблица 5. Качество и скорость"));
c.push(table(["Алгоритм", "Бит", "Доля единиц", "Младший бит", "Хи-квадрат (байты)", "ООА, млн/с", "SOA, млн/с"], rowsStat, [2000, 600, 1300, 1300, 1700, 1369, 1369], { size: 18 }));
c.push(SPACER());
c.push(P("**Анализ.** Доля единиц у всех шести вариантов лежит в пределах 0,4994–0,5003, то есть побитовое распределение сбалансировано. По равномерности байтов Xorshift заметно лучше: у 64-битного варианта значение хи-квадрат 234 (в норме), у 32-битного 316 (на границе 1 %-го порога). У LFSR значения хи-квадрат существенно выше (от 385 до 1728): генератор выдаёт состояние регистра целиком, а последовательные состояния LFSR сдвинуты друг относительно друга на один разряд и потому сильно коррелированы. Это известное свойство, а не ошибка реализации: LFSR используют как источник отдельных битов (младший бит сбалансирован), а не как поставщик целых чисел. Для генерации целых чисел общего назначения предпочтителен Xorshift."));
c.push(P(`**Скорость.** Быстрее всех Xorshift в режиме SOA (${rng(xs, "soa")} млн значений в секунду); у обеих конфигураций LFSR скорость заметно ниже (${rng(lf, "soa")} млн/с в SOA и ${rng(lf, "ooa")} млн/с в ООА). Причина – в реализации, сгенерированной ИИ по скелету функции шага: степень полинома (разрядность регистра) вычисляется побитовым циклом при каждом вызове, до 32 или 64 итераций на шаг, тогда как у Xorshift такого цикла нет. Это сознательно простое и проверяемое решение; в режиме ООА степень можно вычислить один раз в Init, что дало бы заметный выигрыш. Для раздела 3 анализ производительности не является обязательным; данные приведены для сравнения SOA и ООА (раздел 7).`));

// 7
c.push(H1("7. Сравнение подходов SOA и ООА"));
c.push(P("Оба подхода реализованы одновременно в каждом компоненте (требование направления 2 дополнительных баллов), демонстрируются в клиенте и в юнит-тестах: последовательности через SOA и через ООА совпадают для всех шести комбинаций алгоритма и разрядности."));
c.push(CAP("Таблица 6. SOA и ООА"));
const szv = Object.values(sizes);
const objSize = szv[0] + " (Fibonacci и Galois) и " + szv[2] + " (Xorshift)";
c.push(table(["Критерий", "SOA (IEcoPseudoRandomService1)", "ООА (IEcoPseudoRandom1)"], [
  ["Где хранится состояние", "У клиента (в вызывающем коде); компонент состояния не помнит", "Внутри объекта: поля состояния, маски, разрядности и признак инициализации"],
  ["Вызов", "Step32(me, state, param, &new): 4 аргумента, клиент каждый раз передаёт состояние и маску", "Next(me, &value): 2 аргумента после однократного Init"],
  ["Память", `В компоненте ничего кроме общего объекта; у клиента 4 или 8 байт на генератор. Размер самого объекта компонента: ${objSize} байт`, `Тот же объект (${objSize} байт): по сравнению с шаблонным объектом визарда (64 байта) добавлены указатель второй таблицы (8 байт) и поля состояния (24 байта у LFSR – состояние, маска, разрядность и признак инициализации; 16 байт у Xorshift – без маски)`],
  ["Скорость (млн/с, 32 бит)", `Fibonacci ${data.stats[0].soa.toFixed(0)}, Galois ${data.stats[2].soa.toFixed(0)}, Xorshift ${data.stats[4].soa.toFixed(0)}`, `Fibonacci ${data.stats[0].ooa.toFixed(0)}, Galois ${data.stats[2].ooa.toFixed(0)}, Xorshift ${data.stats[4].ooa.toFixed(0)} (дополнительная проверка инициализации и доступ к полям объекта)`],
  ["Параллельное использование", "Безопасно без блокировок: компонент не изменяет данные; можно вести сколько угодно независимых потоков состояний", "Один объект – один поток состояния; для нескольких потоков нужны несколько объектов или синхронизация"],
  ["Удобство", "Нужна ручная передача состояния; удобно для функциональных, воспроизводимых вычислений", "Проще клиентский код (Init и Next); естественно для последовательного потребления чисел"],
], [2200, 3700, 3738]));
c.push(SPACER());
c.push(P(`**Вывод.** SOA даёт максимальную гибкость и потокобезопасность при небольшой нагрузке на клиента; ООА упрощает код клиента и инкапсулирует состояние, платя от ${slowMin} до ${slowMax} % скорости на дополнительные проверки и косвенность. Поскольку обе возможности предоставляет один и тот же объект, накладные расходы памяти у них общие: «бесплатным» по памяти SOA в такой реализации не является.`));

// 8
c.push(H1("8. Инжиниринг промтов и автоматизация разработки"));
c.push(H2("8.1. Методика"));
c.push(P("Код компонентов сгенерирован ИИ-агентами; при этом разрабатывался и проверялся системный промт, а не велась свободная переписка. Использована следующая схема. Системный промт собирается из модулей курса (Common, Roles, Skills, Stack: acom, c) сборщиком `builder.py` с добавлением собственного модуля `Stack/eco-prng.md` (приложение А). Для проверки промта в роли исполнителя запускался ИИ-агент (модель Claude Haiku, слабая модель была выбрана намеренно: штатный агент курса рассчитан на gpt-3.5-turbo, а разница между промтами на сильной модели почти не видна) без какого-либо контекста о задаче, кроме файла промта, текста задания и чистой копии проекта, сгенерированной eco-wizard. Результат оценивался одним и тем же инструментом: сборка xcodebuild, контрактные тесты с эталонной моделью, статическая проверка добавленных строк на запрещённые конструкции (libc, стандартные типы, `//`, суффиксы ULL) и проверка уникальности глобальных символов. Каждая найденная ошибка разбиралась по исходному коду до причины, и правило, закрывающее причину, добавлялось в следующую версию промта."));
c.push(H2("8.2. Исходный промт курса и найденный в нём дефект"));
c.push(P("В агенте `creative_c_coder.md` модуль роли указан как `Roles/coder-с.md`, но буква «с» в имени – кириллическая (U+0441), тогда как реальный файл называется `coder-c.md` с латинской «c». Сборщик выводит предупреждение «модуль не найден» и пропускает роль coder, поэтому штатно собранный промт курса не содержит инструкций роли разработчика. Версии P1–P9 используют исправленное имя."));
c.push(H2("8.3. История итераций"));
c.push(P("Версии промта P0–P9 и результаты 29 прогонов подробно записаны в журнале `prompts/experiment_log.md`. Каждая ячейка таблицы соответствует одному запуску агента (это история исправлений, а не статистическая оценка: при повторных запусках слабая модель даёт разброс)."));
c.push(CAP("Таблица 7. Версии промта и результаты (Haiku, если не указано иное)"));
const iter = [
  ["P0 штатный курса", "роль coder не подключилась (кириллическая «с»)", "Sonnet: пройден; Haiku: падение (Abort trap)", "–", "–"],
  ["P1", "исправлена роль; правила C89, запрет libc, проверки, поправка `me` во вторичных интерфейсах", "пройден", "падение (Bus error: агент переписал шаблонную фабрику)", "пройден"],
  ["P2", "адрес поля-таблицы и значение поля; не переписывать шаблон", "ошибка ООА (усечение)", "ошибка Service (усечение по `bits`)", "падение (нумерация интерфейсов с единицы)"],
  ["P3", "нумерация с нуля; одна функция шага; обязательное усечение", "компонент не найден (QueryInterface знал только себя)", "ошибка Service (усечение по `bits`)", "пройден"],
  ["P4", "полнота QueryInterface; разрядность из маски", "ошибка Service (усечение по `bits`)", "пройден", "пройден"],
  ["P5", "скелет функции шага LFSR", "падение при освобождении (статическая строка в `m_Name`)", "падение (удалены поля шаблона из структуры)", "пройден"],
  ["P6", "сохранность структуры; имя – статическая строка", "пройден", "пройден", "ошибка (усечение только в конце формулы)"],
  ["P7", "усечение после каждого сдвига влево", "пройден", "пройден", "пройден"],
  ["P8", "уникальность глобальных имён", "пройден", "пройден", "пройден"],
  ["P9 финальный", "явное правило `me` и разделение in/out (по условию бонуса)", "пройден", "пройден", "пройден"],
];
c.push(table(["Промт", "Что добавлено", "Fibonacci", "Galois", "Xorshift"], iter, [1250, 2788, 1900, 1900, 1800], { size: 16, fillFn: (r, i) => (i >= 2 && /^пройден/.test(r[i]) ? "E2F0D9" : (i >= 2 && r[i] !== "–" && !/^Sonnet/.test(r[i]) ? "FBE4D5" : undefined)) }));
c.push(SPACER());
c.push(P("**Какие ошибки делал ИИ и как промт их исправил (по причинам):**"));
c.push(N("**Преобразование `me` во втором интерфейсе.** Агент приводил `me` метода второго интерфейса прямо к объекту, не учитывая, что это адрес второго поля-таблицы; память портилась (падение). Исправлено правилом поправки с примером; затем выяснилось, что формулировка «k-й интерфейс» понималась с единицы и поправка ошибочно применялась и к первому интерфейсу. Нумерация переписана с нуля с готовыми формулами для двух интерфейсов."));
c.push(N("**Усечение до разрядности регистра.** Состояние усекалось по разрядности вызова (32/64), а не по степени полинома из маски; в другом случае усечение выполнялось только при `width < bits` и в 32-битном режиме бит 32 оставался. Словесное правило с числовым примером не удержало слабую модель; сработал готовый скелет функции шага (подстановка маски – степень – маска разрядности – усечение входа – формула – усечение результата). Позже выявилась и ошибка самого скелета: у Xorshift усекать нужно результат каждого сдвига влево, а не только итог."));
c.push(N("**Дублирование формулы** в Service и в ООА приводило к расхождению копий. Введено правило единственной функции шага, которую вызывают Step32, Step64 и Next."));
c.push(N("**Неполный QueryInterface:** каждый интерфейс знал только себя и IEcoUnknown, фабрика не могла выдать второй интерфейс (ошибка компонента не найден). Правило: три ветки в каждой функции QueryInterface."));
c.push(N("**Лишняя «уборка» шаблона:** удалялись поля `Init/Create/Delete` из структуры объекта (Bus error), переписывалась рабочая фабрика, в `m_Name` записывалась статическая строка, которую шаблонный `Delete` освобождал через аллокатор (аварийное завершение при освобождении). Добавлены правила о сохранности структуры и фабрики и о статической строке имени."));
c.push(N("**Конфликт глобальных имён при линковке** (найден только при сборке клиента из трёх компонентов): библиотеки Fibonacci и Galois экспортировали одинаковые имена таблиц функций; линковщик связал объект Galois с таблицами Fibonacci, и Galois выдавал последовательность Fibonacci. Каждый компонент по отдельности проходил все тесты. Добавлено правило суффикса CID у всех внешних имён и проверка уникальности символов в инструменте оценки."));
c.push(P("Две из найденных ошибок (нумерация интерфейсов и усечение только в конце формулы) оказались недостатками самих правил промта, а не «глупостью» модели; это важный практический вывод: формулировку правила нужно проверять на исполнителе, а не только на авторе."));
c.push(H2("8.4. Финальный промт"));
c.push(P("Финальный текст системного промта – сборка P9 (модули курса плюс собственный `Stack/eco-prng.md`); он приведён в приложении А полностью и лежит в файле `prompts/P9_ours.md`. Собственный модуль (раздел «Stack/eco-prng.md» внутри приложения) содержит всё, что требует условие: роль, правила интерфейсов (`me`, in/out), запрет libc, проверки и крайние случаи. Состав промта: роль системного программиста ACOM; правила интерфейсов АОМК (передача указателя `me` первым параметром, разделение параметров на `/* in */`, `/* out */` и `/* in | out */`, подсчёт ссылок и полнота QueryInterface); категорический запрет подключения и вызовов функций libc, включая неявные (`memcpy`, присваивание структур); автоматическая проверка указателей на NULL и крайние математические случаи; скелет функции шага; чек-лист самопроверки перед сдачей."));
c.push(H2("8.5. Пример эталонного сгенерированного кода"));
c.push(P("Шаговая функция компонента Galois LFSR, сгенерированная агентом по финальному промту P9 (без правок), и вспомогательная функция степени полинома:"));
c.push(...code(degLines.concat([""], coreLines), 16));
c.push(SPACER());
c.push(H2("8.6. Ограничения оценки"));
c.push(P("Каждый вариант промта проверен одним запуском на задачу, поэтому таблица 7 показывает историю причин и исправлений, а не вероятность успеха. Финальный промт дал три успешных результата подряд (версии P7, P8 и P9 по три из трёх), но надёжность на большом числе запусков не измерялась. Исполнители – ИИ-агенты, поэтому все результаты проверены автоматически и разбором кода, и итоговая ответственность за корректность лежит на проверке, а не на генерации."));

// 9
c.push(H1("9. Выводы"));
c.push(B("Разработаны и проверены три независимых компонента АОМК (Fibonacci LFSR, Galois LFSR, Xorshift) с общим контрактом из двух интерфейсов; переключение алгоритма в клиенте выполняется заменой CID."));
c.push(B("Реализованы и сопоставлены два архитектурных подхода – SOA (без состояния) и ООА (с состоянием); последовательности совпадают для всех комбинаций, различия по памяти, скорости и удобству описаны в разделе 7."));
c.push(B("Значения компонентов совпадают с независимой эталонной моделью (0 расхождений из 14 сравнений) и с опубликованными значениями Xorshift; период 16-битных регистров равен 65535. Xorshift превосходит LFSR по равномерности байтов, LFSR остаётся сбалансированным источником отдельных бит."));
c.push(B("Системный промт разработан итерационно (версии P0–P9): каждая версия исправляет реально наблюдавшиеся ошибки ИИ; обнаружены дефект сборки промтов курса и недостатки собственных формулировок."));

// Appendix A
c.push(H1("Приложение А. Полный текст финального системного промта (версия P9)", true));
c.push(...code(promptFull.filter((l, i) => !(i < 8 && /^(<!--|AGENT:|LANG:|MODEL:|TEMPERATURE:|-->)/.test(l))), 13));
c.push(H1("Приложение Б. Состав поставки", true));
c.push(table(["Что", "Где"], [
  ["Компоненты (исходный код и сборка)", "~/Dev/Eco.FibonacciLFSR1, ~/Dev/Eco.GaloisLFSR1, ~/Dev/Eco.Xorshift1 (SourceFiles, HeaderFiles, SharedFiles, BuildFiles/Mac)"],
  ["Клиент «Калькулятор» и демонстрация", "~/Dev/Eco.Lab1 (UnitTestFiles/SourceFiles/EcoLab1.c)"],
  ["Общие интерфейсы и IDL", "~/Dev/Eco.PseudoRandom/SharedFiles"],
  ["Эталонная модель и генератор векторов", "~/Dev/Eco.PseudoRandom/reference"],
  ["Контрактные тесты, установка, оценка", "~/Dev/Eco.PseudoRandom/tests"],
  ["Промты, задания, журнал экспериментов", "~/Dev/Eco.PseudoRandom/prompts"],
  ["Сбор данных и построение отчёта", "~/Dev/Eco.PseudoRandom/report"],
  ["Проекты прогонов финального промта P9 и архив версий на основе P8", "~/Dev/Eco.Experiments (P9/run1–run3, shipped_P8)"],
], [3600, 6038]));

/* ---------- document ---------- */
const doc = new Document({
  creator: "Lab1", title: "Лабораторная работа 1. Генераторы псевдослучайных чисел (АОМК)",
  styles: {
    default: { document: { run: { font: FONT, size: 24 } } },
    paragraphStyles: [
      { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true, run: { size: 30, bold: true, font: FONT }, paragraph: { spacing: { before: 360, after: 160 }, outlineLevel: 0 } },
      { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true, run: { size: 26, bold: true, font: FONT }, paragraph: { spacing: { before: 240, after: 120 }, outlineLevel: 1 } },
    ],
  },
  numbering: {
    config: [
      { reference: "bullets", levels: [{ level: 0, format: LevelFormat.BULLET, text: "•", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
      { reference: "numbers", levels: [{ level: 0, format: LevelFormat.DECIMAL, text: "%1.", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
    ],
  },
  sections: [{
    properties: { page: { size: { width: 11906, height: 16838 }, margin: { top: 1134, right: 1134, bottom: 1134, left: 1134 } } },
    footers: { default: new Footer({ children: [new Paragraph({ alignment: AlignmentType.CENTER, children: [new TextRun({ children: [PageNumber.CURRENT], font: FONT, size: 20 })] })] }) },
    children: c,
  }],
});

Packer.toBuffer(doc).then((buf) => {
  const out = path.join(__dirname, "Report_Lab1_PRNG.docx");
  fs.writeFileSync(out, buf);
  console.log("written", out, buf.length, "bytes");
});
