/*******************************************************************************
  FILE: jottermodel.h
  AUTOR: ixotum
  DATE: 2012-09-04 20:27:55
  DESCRIPTION: This file contains definition of JotterModel class
*******************************************************************************/
#ifndef JOTTERMODEL_H
#define JOTTERMODEL_H

#include <QAbstractItemModel>

#include "jot.h"

class JotterModel : public QAbstractItemModel
{
public:
  JotterModel();
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool insertRows(int row, int count, const QModelIndex &parent);
  bool insertColumns(int column, int count, const QModelIndex &parent);
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

private:
  Jot *getJot(const QModelIndex &index) const;

  Jot *rootJot;
};

#endif // JOTTERMODEL_H
