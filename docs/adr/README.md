# Architecture Decision Records

This directory contains Architecture Decision Records (ADRs) for the OpenRetro Portable Firmware project.

## What is an ADR?

An ADR documents a significant architectural or technical decision made in the project. It captures not just what was decided, but the context, reasoning, and consequences—creating a durable record of why the system is the way it is.

ADRs help:
- **Onboard new contributors** who need to understand design rationale
- **Prevent rehashing** of old decisions
- **Track evolution** of the architecture over time
- **Document tradeoffs** and constraints that aren't obvious from code

## When to Write an ADR

Write an ADR for decisions that:
- Are significant and have lasting impact on the architecture
- Involve tradeoffs or alternatives that were considered
- May not be obvious to someone reading the code
- Affect multiple components or the overall system shape

Examples: choice of communication protocol, memory layout decisions, MCU selection rationale, approach to bootloader design, debugging strategy.

Do *not* write ADRs for routine implementation details, bug fixes, or minor refactors.

## How to Create a New ADR

1. Copy `0000-template.md` to a new file: `NNNN-short-title-kebab-case.md`
   - Use the next sequential number (e.g., `0001-`, `0002-`)
   - Use lowercase, kebab-case for the title
   
2. Fill in all sections of the template, following the guidance in each

3. Set **Status** to "Proposed" initially

4. Commit the ADR with a clear message:
   ```
   docs: add ADR NNNN: short description
   ```

5. When the decision is finalized and approved, update Status to "Accepted"

## Status Values

- **Proposed**: Decision has been drafted but not yet finalized
- **Accepted**: Decision has been agreed upon and is now in effect
- **Deprecated**: Decision is no longer in effect, but kept for historical context
- **Superseded**: Decision has been replaced by a newer ADR (reference the new one)

## Index

<!-- ADRs will be listed here as they are created -->

| # | Title | Status |
|---|-------|--------|
|   |       |        |
