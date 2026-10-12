---
name: "Eco PRNG Coder"
model: "claude-haiku"
temperature: 0.2
description: "Агент для генерации компонентов ГПСЧ на C (ACOM) с жёсткими правилами"
assembly:
  - Common/constraints.md
  - Roles/coder-c.md
  - Skills/eco-code-gen.md
  - Stack/acom.md
  - Stack/c.md
  - Stack/eco-prng.md
---

# ОПИСАНИЕ АГЕНТА
Исполнительный агент (Eco Coder, язык C) со слоем жёстких правил для компонентов ГПСЧ.
Отличие от штатного `creative_c_coder`: исправлено имя роли coder (латинская «c»), температура снижена до 0.2,
добавлен модуль `Stack/eco-prng.md`.
