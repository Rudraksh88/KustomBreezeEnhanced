#ifndef breeze_h
#define breeze_h

/*
 * Copyright 2014  Hugo Pereira Da Costa <hugo.pereira@free.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "breezesettings.h"

#include <QList>
#include <QSharedPointer>
#include <QString>

namespace Breeze
{
    //* convenience typedefs
    using InternalSettingsPtr = QSharedPointer<InternalSettings>;
    using InternalSettingsList = QList<InternalSettingsPtr>;
    using InternalSettingsListIterator = QListIterator<InternalSettingsPtr>;

    namespace ButtonSizing
    {
        static constexpr int Minimum = 10;
        static constexpr int Default = 28;
        static constexpr int Maximum = 64;

        inline int fromLegacyIndex(int value)
        {
            // Pixel equivalents of the former grid-unit presets at the
            // standard 18 px Plasma grid unit.
            static constexpr int sizes[] = {18, 25, 28, 36, 45};
            return value >= 0 && value < 5 ? sizes[value] : Default;
        }

        inline int fromConfigValue(const QString &value, int fallback)
        {
            if (value.compare(QStringLiteral("ButtonTiny"), Qt::CaseInsensitive) == 0)
                return fromLegacyIndex(0);
            if (value.compare(QStringLiteral("ButtonSmall"), Qt::CaseInsensitive) == 0)
                return fromLegacyIndex(1);
            if (value.compare(QStringLiteral("ButtonDefault"), Qt::CaseInsensitive) == 0)
                return fromLegacyIndex(2);
            if (value.compare(QStringLiteral("ButtonLarge"), Qt::CaseInsensitive) == 0)
                return fromLegacyIndex(3);
            if (value.compare(QStringLiteral("ButtonVeryLarge"), Qt::CaseInsensitive) == 0)
                return fromLegacyIndex(4);

            bool valid = false;
            const int numericValue = value.toInt(&valid);
            if (!valid)
                return fallback;

            // ItemEnum could also store an out-of-range value numerically.
            // Values below the new slider minimum therefore represent a
            // legacy preset index, not a pixel size.
            if (numericValue >= 0 && numericValue < 5)
                return fromLegacyIndex(numericValue);

            return qBound(Minimum, numericValue, Maximum);
        }
    }

    namespace TitleBarIconSizing
    {
        // Preserve the scale used by the former default 28 px button and its
        // padding without tying the centered icon to live button geometry.
        static constexpr int ReferenceHeight = 36;
    }

    //* metrics
    enum Metrics
    {

        //* corner radius (pixels)
        Frame_FrameRadius = 0,

        //* titlebar metrics, in units of small spacing
        // defaulting now to small spacing
        TitleBar_TopMargin = 1, // 2,
        TitleBar_BottomMargin = 1, // 2,
        TitleBar_SideMargin = 1, // 2,
        TitleBar_ButtonSpacing = 1, // 2,

        // shadow dimensions (pixels)
        Shadow_Overlap = 3,

    };

    //* standard pen widths
    namespace PenWidth
    {
        /* Using 1 instead of slightly more than 1 causes symbols drawn with
        * pen strokes to look skewed. The exact amount added does not matter
        * as long as it isn't too visible.
        */
        // The standard pen stroke width for symbols.
        static constexpr qreal Symbol = 1.01;
    }

    //* exception
    enum ExceptionMask
    {
        None = 0,
        BorderSize = 1<<4,
        ButtonSize = 1<<5,
        ButtonSpacing = 1<<6,
        ButtonPadding = 1<<7,
        HOffset = 1<<8,
        TitleBarIconSize = 1<<9,
        TitleBarIconSpacing = 1<<10
    };
}

#endif
