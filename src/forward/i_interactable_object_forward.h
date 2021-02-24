#ifndef I_INTERACTABLE_OBJECT_FORWARD_H
#define I_INTERACTABLE_OBJECT_FORWARD_H

#include <memory>
#include <vector>

class IInteractableObject;
using IInteractObjShared         = std::shared_ptr<IInteractableObject>;
using IInteractObjWeak           = std::weak_ptr<IInteractableObject>;
using IInteractObjSharedConst    = std::shared_ptr<const IInteractableObject>;
using IInteractObjWeakConst      = std::weak_ptr<const IInteractableObject>;

using IInteractObjWeakVec          = std::vector<IInteractObjWeak>;
using IInteractObjWeakConstVec     = std::vector<IInteractObjWeakConst>;
using IInteractObjSharedVec        = std::vector<IInteractObjShared>;
using IInteractObjSharedConstVec   = std::vector<IInteractObjSharedConst>;

#endif // I_INTERACTABLE_OBJECT_FORWARD_H
