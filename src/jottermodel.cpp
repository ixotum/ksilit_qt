/*******************************************************************************
  FILE: jottermodel.cpp
  AUTOR: ixotum
  DATE: 2012-09-04 20:28:18
  DESCRIPTION: This file contains implementation of JotterModel class
*******************************************************************************/

#include "jottermodel.h"

JotterModel::JotterModel()
{
}

QModelIndex JotterModel::index(int row, int column, const QModelIndex &parent) const {
  //dummy
}

QModelIndex JotterModel::parent(const QModelIndex &child) const {
  //dummy
}

int JotterModel::rowCount(const QModelIndex &parent) const {
  //dummy
}

int JotterModel::columnCount(const QModelIndex &parent) const {
  //dummy
}

QVariant JotterModel::data(const QModelIndex &index, int role) const {
  //dummy
}
