#pragma once

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>

//=============================================================================
class GitIgnoreMatch {
public:
    GitIgnoreMatch(const QString& dirPath) {
        QDir dir(dirPath);
        while (true) {
            QString candidate = dir.absoluteFilePath(".gitignore");
            if (QFileInfo::exists(candidate)) {
                gitIgnoreDir_ = dir.absolutePath();
                parse_(candidate);
                return;
            }
            if (!dir.cdUp()) break;
        }
    }

    bool hasGitIgnore() const { return !gitIgnoreDir_.isEmpty(); }

    // match fileName (just name, not path) against rules
    // isDir: append '/' semantics for dir-only patterns
    bool isIgnored(const QString& fileName, bool isDir) const {
        if (rules_.isEmpty()) return false;

        bool ignored = false;
        for (const Rule_& r : rules_) {
            bool matches = false;

            if (r.dirOnly && !isDir) {
                continue;  // pattern like "dir/" only matches directories
            }

            if (r.hasSlash) {
                // pattern contains '/' — match against relative path
                // for top-level files, fileName is the relative path
                matches = matchGlob_(r.pattern, fileName);
            } else {
                matches = matchGlob_(r.pattern, fileName);
            }

            if (matches) {
                ignored = !r.negated;
            }
        }
        return ignored;
    }

private:
    struct Rule_ {
        QString pattern;
        bool negated  = false;
        bool dirOnly  = false;  // pattern ended with '/'
        bool hasSlash = false;  // pattern contains '/' (anchored to gitignore dir)
    };

    QList<Rule_> rules_;
    QString gitIgnoreDir_;

    void parse_(const QString& gitignorePath) {
        QFile f(gitignorePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QTextStream in(&f);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) continue;

            Rule_ rule;

            // negation
            if (line.startsWith('!')) {
                rule.negated = true;
                line = line.mid(1);
            }

            // dir-only pattern (trailing '/')
            if (line.endsWith('/')) {
                rule.dirOnly = true;
                line.chop(1);
            }

            // leading '/' means anchored — remove it but mark hasSlash
            if (line.startsWith('/')) {
                rule.hasSlash = true;
                line = line.mid(1);
            }

            // check if pattern contains '/' (path separator)
            if (line.contains('/')) {
                rule.hasSlash = true;
            }

            rule.pattern = line;
            rules_.append(rule);
        }
    }

    // simple glob: '*' matches any chars except '/', '?' matches one char
    bool matchGlob_(const QString& pattern, const QString& text) const {
        int pi = 0, ti = 0;
        int pLen = pattern.length(), tLen = text.length();
        int starPi = -1, starTi = -1;

        while (ti < tLen) {
            if (pi < pLen && pattern[pi] == '*') {
                starPi = pi++;
                starTi = ti;
            } else if (pi < pLen && (pattern[pi] == '?' || pattern[pi] == text[ti])) {
                pi++;
                ti++;
            } else if (starPi >= 0) {
                pi = starPi + 1;
                ti = ++starTi;
            } else {
                return false;
            }
        }

        while (pi < pLen && pattern[pi] == '*') pi++;
        return pi == pLen;
    }
};
