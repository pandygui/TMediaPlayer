/*
Copyright (C) 2012-2016 Teddy Michel

This file is part of TMediaPlayer.

TMediaPlayer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TMediaPlayer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TMediaPlayer. If not, see <http://www.gnu.org/licenses/>.
*/

#include "IWidgetCriterion.hpp"


IWidgetCriterion::IWidgetCriterion(CMainWindow * mainWindow, QWidget * parent) :
QWidget      (parent),
m_type       (ICriterion::TypeInvalid),
m_condition  (ICriterion::CondInvalid),
m_mainWindow (mainWindow)
{
    Q_CHECK_PTR(m_mainWindow);
}


IWidgetCriterion::~IWidgetCriterion()
{

}
