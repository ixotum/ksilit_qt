/*******************************************************************************
  FILE: jottermodel.h
  AUTOR: ixotum
  DATE: 2012-09-04 20:27:55
  DESCRIPTION: This file contains definition of JotterModel class
*******************************************************************************/
#ifndef JOTTERMODEL_H
#define JOTTERMODEL_H

#include <QAbstractItemModel>

class JotterModel : public QAbstractItemModel
{
public:
  JotterModel();
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
};

#endif // JOTTERMODEL_H
