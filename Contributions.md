# Contributions Guide

Thanks for your interest in improving Memory Card Flip Game! This guide is a quick checklist for contributors. For full details, see `CONTRIBUTING.md`.

## TL;DR Workflow

- Fork the repo and create a feature branch
- Make small, focused changes with clear commit messages
- Build and run the game locally; add/update tests when applicable
- Ensure code style and docs are updated
- Open a PR with a clear description and screenshots/GIFs when visual

## Branch Naming

Use short, kebab-cased names:
- feature/settings-menu
- fix/crash-on-exit
- chore/update-readme

## Commit Message Convention

Follow Conventional Commits:
- feat: add settings menu
- fix: handle null audio device initialization
- refactor: extract card rendering logic
- docs: update README with setup steps
- chore: bump raylib version to 5.0

Include a short summary line, then bullet points for details if needed.

## Code Style (C++)

- C++17, 4-space indentation, no tabs
- Keep lines ≤ 100 chars where practical
- Names: camelCase (vars/functions), PascalCase (classes), UPPER_SNAKE_CASE (constants)
- Add comments for non-obvious logic
- Prefer RAII; avoid raw new/delete

## PR Quality Checklist

- [ ] Compiles without warnings (or explains why unavoidable)
- [ ] Runs without crashes and regressions
- [ ] Tests added/updated (when changing logic)
- [ ] No unused files or dead code
- [ ] Documentation updated (README/CONTRIBUTING as needed)
- [ ] Screenshots/GIFs for UI changes

## Issue Etiquette

- Search existing issues before creating a new one
- Provide repro steps, expected vs actual, and environment details
- Keep discussions respectful and focused on the technical topic

## Hacktoberfest Tips

- Look for issues labeled hacktoberfest, good-first-issue, help-wanted
- Aim for meaningful contributions (not just minor typo fixes)
- Ask for clarification if scope is unclear before large changes

## Getting Help

- Open an issue with the `question` label
- Tag maintainers in discussions for blockers
- Email (as listed in README) for project-level questions

Happy contributing! 🎉