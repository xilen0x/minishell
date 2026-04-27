#!/bin/bash
# by castorga

# Checks for uncommitted changes in the current git repository,
# prompts the user for confirmation and a commit message,
# then stages, commits and pushes all changes to the remote.


# Check if we are in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "Error: not inside a git repository."
    exit 1
fi

# Check for uncommitted changes (tracked and untracked)
if [ -z "$(git status -s)" ]; then
    echo "No changes to commit. Repository is up to date."
    exit 0
fi

echo "Changes detected:"
git status -s
echo ""

# Ask for confirmation before adding everything
echo -n "Add all changes and commit? [y/N]: "
read confirm
if [ "$confirm" != "y" ] && [ "$confirm" != "Y" ]; then
    echo "Aborted."
    exit 0
fi

# Stage all changes
git add .

# Show what is staged
echo ""
echo "Staged changes:"
git diff --cached --stat
echo ""

# Ask for commit message (non-empty)
while true; do
    echo -n "Commit message: "
    read message
    if [ -n "$message" ]; then
        break
    fi
    echo "Error: commit message cannot be empty."
done

# Commit
if ! git commit -m "$message"; then
    echo "Error: commit failed."
    exit 1
fi

# Push
echo ""
echo "Pushing to remote..."
if ! git push; then
    echo "Error: push failed. Your commit is saved locally."
    exit 1
fi

echo ""
echo "Changes committed and pushed successfully."
