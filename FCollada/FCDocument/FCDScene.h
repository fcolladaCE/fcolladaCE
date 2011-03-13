// CODE ADDED

#ifndef FCDSCENE_H_
#define FCDSCENE_H_

#ifndef __FCD_OBJECT_H_
#include "FCDocument/FCDObject.h"
#endif // __FCD_OBJECT_H_
#ifndef _FU_PARAMETER_H_
#include "FUtils/FUParameter.h"
#endif // _FU_PARAMETER_H_

class FCDExtra;

class FCOLLADA_EXPORT FCDScene : public FCDObject
{
private:
	DeclareObjectType(FCDObject);
	
	DeclareParameterRef(FCDExtra, extra, FC("Extra Tree"));
	
	FUObject* parent;
	
public:	
	/**	Constructor.
	@param document The COLLADA document that owns the scene. */
	FCDScene(FCDocument* document, FUObject* _parent);
	
	
	/** Destructor. */
	virtual ~FCDScene();
	
	
		/** Retrieves the extra information tree for this entity.
		The prefered way to save extra information in FCollada is at
		the entity level. Use this extra information tree to store
		any information you want exported and imported back.
		@return The extra information tree. */
	FCDExtra* GetExtra() { return extra; }
	const FCDExtra* GetExtra() const { return extra; } /**< See above. */
	
		/** Clones the asset structure into another scene structure.
		@param clone The scene structure that will become the copy of this scene.
			When this pointer is NULL, a new scene structure will be created.
		@param cloneAllContributors Whether all the contributors of this scene
			should be copied into the clone.
		@return The clone. */
	FCDScene* Clone(FCDScene* clone) const;
};

#endif /*FCDSCENE_H_*/
