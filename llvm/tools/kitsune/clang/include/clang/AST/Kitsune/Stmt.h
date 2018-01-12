/**
  ***************************************************************************
  * Copyright (c) 2017, Los Alamos National Security, LLC.
  * All rights reserved.
  *
  *  Copyright 2010. Los Alamos National Security, LLC. This software was
  *  produced under U.S. Government contract DE-AC52-06NA25396 for Los
  *  Alamos National Laboratory (LANL), which is operated by Los Alamos
  *  National Security, LLC for the U.S. Department of Energy. The
  *  U.S. Government has rights to use, reproduce, and distribute this
  *  software.  NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY,
  *  LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LIABILITY
  *  FOR THE USE OF THIS SOFTWARE.  If software is modified to produce
  *  derivative works, such modified software should be clearly marked,
  *  so as not to confuse it with the version available from LANL.
  *
  *  Additionally, redistribution and use in source and binary forms,
  *  with or without modification, are permitted provided that the
  *  following conditions are met:
  *
  *    * Redistributions of source code must retain the above copyright
  *      notice, this list of conditions and the following disclaimer.
  *
  *    * Redistributions in binary form must reproduce the above
  *      copyright notice, this list of conditions and the following
  *      disclaimer in the documentation and/or other materials provided
  *      with the distribution.
  *
  *    * Neither the name of Los Alamos National Security, LLC, Los
  *      Alamos National Laboratory, LANL, the U.S. Government, nor the
  *      names of its contributors may be used to endorse or promote
  *      products derived from this software without specific prior
  *      written permission.
  *
  *  THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND
  *  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  *  DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR
  *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  *  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  *  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  *  SUCH DAMAGE.
  *
  ***************************************************************************/

#ifndef KITSUNE_LLVM_CLANG_AST_STMT_H
#define KITSUNE_LLVM_CLANG_AST_STMT_H

#include "clang/AST/Stmt.h"

namespace clang {

// +===== Kitsune

class KitsuneStmt : public Stmt {
public:
  enum KitsuneStmtKind{
    Forall
  };

  KitsuneStmt(KitsuneStmtKind K)
  : Stmt(KitsuneStmtClass),
    Kind(K){}

  virtual ~KitsuneStmt(){}

  /// \brief Build an empty for statement.
  explicit KitsuneStmt(EmptyShell Empty) : Stmt(KitsuneStmtClass, Empty) { }

  static bool classof(const Stmt *T) {
    return T->getStmtClass() == KitsuneStmtClass;
  }

  KitsuneStmtKind kind() const{
    return Kind;
  }
  
  static bool classof(const KitsuneStmt *){ return true; }
  
  virtual child_range children(){
    return child_range(child_iterator(), child_iterator());
  }

  SourceLocation getLocStart() const LLVM_READONLY{
    return StartLoc;
  }
  
  void setLocStart(SourceLocation Loc){
    StartLoc = Loc;
  }
  
  SourceLocation getLocEnd() const LLVM_READONLY{
    return EndLoc;
  }
  
  void setLocEnd(SourceLocation Loc){
    EndLoc = Loc;
  }

private:
  KitsuneStmtKind Kind;
  SourceLocation StartLoc;
  SourceLocation EndLoc;
};

class ForallStmt : public KitsuneStmt{
public:
  ForallStmt(const ASTContext &C, VarDecl *IndVar, Expr *Size,
             Stmt *Body, SourceLocation FL, SourceLocation LP,
             SourceLocation RP)
  : KitsuneStmt(KitsuneStmt::Forall),
  IndVar(IndVar),
  Size(Size),
  Body(Body),
  LP(LP),
  RP(RP){}

  VarDecl *getIndVar() { return IndVar; }
  Expr *getSize()  { return Size; }
  Stmt *getBody() { return Body; }

  const VarDecl *getIndVar() const { return IndVar; }
  const Expr *getSize() const { return Size; }
  const Stmt *getBody() const { return Body; }

private:
  VarDecl *IndVar;
  Expr *Size;
  Stmt *Body;
  SourceLocation LP;
  SourceLocation RP;
};

// +============

}  // end namespace clang

#endif
