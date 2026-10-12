# ЗАДАЧА: компонент Eco.Xorshift1

Проект компонента уже сгенерирован eco-wizard в рабочей папке (путь указан ниже). Нужно реализовать
генератор псевдослучайных чисел Xorshift (Дж. Марсалья).

Компонент обязан реализовывать ДВА интерфейса, уже описанных в `SharedFiles/`
(`IEcoPseudoRandomService1.h/.idl` и `IEcoPseudoRandom1.h/.idl`; их менять нельзя, прочитай контракт в комментариях заголовков):
- `IEcoPseudoRandomService1` — без состояния (SOA): `Step32`, `Step64`;
- `IEcoPseudoRandom1` — с состоянием внутри объекта (OOA): `Init`, `Next`, `get_Name`.

Оба интерфейса принадлежат одному экземпляру компонента (один счётчик ссылок, `QueryInterface` отдаёт любой из них и `IEcoUnknown`).
Шаблонный интерфейс `IEcoXorshift1` с методом `MyFunction` — заглушка визарда, его можно удалить.
CID компонента (`SharedFiles/IdEcoXorshift1.h`) менять нельзя.

## Алгоритм
- Параметр `param` игнорируется (у Xorshift нет маски). Разрядность регистра равна числу бит вызова: `Step32` — 32, `Step64` — 64; в OOA — `bits` из `Init`.
- Шаг 32 бит: `x ^= x << 13; x ^= x >> 17; x ^= x << 5;` (все операции в 32-битном беззнаковом регистре, переполнение усекается).
- Шаг 64 бит: `x ^= x << 13; x ^= x >> 7; x ^= x << 17;` (в 64-битном регистре).
- Состояние `state` усекается до разрядности вызова; нулевое состояние — ошибка `ERR_ECO_INVALIDARG` (у Xorshift ноль — неподвижная точка).
- Значение, возвращаемое генератором, равно новому состоянию (`*newState` / `*value`), для 32-битного режима `Next` возвращает его с нулевым расширением.
- OOA: `Init(seed, param, bits)`: `bits` равно 32 или 64, иначе `ERR_ECO_INVALIDARG`; нулевое (после усечения) `seed` — `ERR_ECO_INVALIDARG`;
  повторный `Init` перезапускает последовательность; `Next` до `Init` — `ERR_ECO_FAIL`; `get_Name` возвращает непустое имя алгоритма.
- Любой указатель-параметр, равный NULL, и `me == NULL` — `ERR_ECO_POINTER`.

## Что сдать
Рабочие исходники компонента в его папке проекта (`SourceFiles/`, `HeaderFiles/`, при необходимости `SharedFiles/Id…`),
которые собираются проектом `AssemblyFiles/Mac/Xcode_v123/EcoXorshift1.xcodeproj` (схема `Static`, конфигурация Debug).
Сборка из терминала: `ECO_FRAMEWORK=$HOME/Dev/Eco.Framework xcodebuild -project EcoXorshift1.xcodeproj -scheme Static -configuration Debug build`
(запускать в `AssemblyFiles/Mac/Xcode_v123`). Не трогай `UnitTestFiles/`. В конце кратко перечисли изменённые файлы.
