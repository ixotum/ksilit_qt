/*******************************************************************************
  FILE: jot.h
  AUTOR: ixotum
  DATE: 2012-09-04 20:26:33
  DESCRIPTION: This file contains definition of Jot class
*******************************************************************************/

#ifndef JOT_H
#define JOT_H

class Jot
{
public:
  Jot(Jot *parent = 0);
  Jot *parent();

private:
  Jot *parentJot;
};

#endif // JOT_H
