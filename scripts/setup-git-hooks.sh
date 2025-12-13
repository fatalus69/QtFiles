#!/usr/bin/env bash
set -e

PROJECT_ROOT="$(git rev-parse --show-toplevel)"

HOOK="$PROJECT_ROOT/.git/hooks/pre-commit"
TARGET="$PROJECT_ROOT/scripts/pre-commit.sh"

if [ -e "$HOOK" ]; then
  echo "pre-commit hook already exists"
  exit 0
fi

ln -s "$TARGET" "$HOOK"
chmod +x "$TARGET"