/*
    SPDX-FileCopyrightText: 2018 Sergio Martins <smartins@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef CLAZY_STATIC_PMF_H
#define CLAZY_STATIC_PMF_H

#include "checkbase.h"

#include <string>

/**
 * See README-static-pmf.md for more info.
 */
class StaticPmf : public CheckBase
{
public:
    explicit StaticPmf(const std::string &name);
    void VisitDecl(clang::Decl *) override;

private:
};

#endif
