// CODE ADDED


#include "StdAfx.h"
#include "FCDocument/FCDScene.h"
#include "FCDocument/FCDocument.h"
#include "FCDocument/FCDExtra.h"



ImplementObjectType(FCDScene);

ImplementParameterObject(FCDScene, FCDExtra, extra, new FCDExtra(parent->GetDocument(), parent));

FCDScene::FCDScene(FCDocument* document, FUObject* _parent):
FCDObject(document)
,InitializeParameterNoArg(extra)
,parent(_parent)
{
	extra= new FCDExtra(document,this);
}

FCDScene::~FCDScene()
{
}

// Clone another scene element.
FCDScene* FCDScene::Clone(FCDScene* clone) const
{
	if (clone == NULL) clone = new FCDScene(const_cast<FCDocument*>(GetDocument()), NULL);

	return clone;
}

