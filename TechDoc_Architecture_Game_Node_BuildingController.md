[Back](TechDoc_Architecture.md)

# Overview #

These nodes aid to compliment Building Controllers by giving the designers the ability to directly talk to the controller's logical entities through the Flow System. Some of the nodes act as "listeners" which simply report back when events occur on the specified controller, letting the designer define a new behavior for that event. Others let the designer directly manipulate the status of the building, such as triggering its power status.

# [Functionality](TechDoc_Architecture_Game_Node_BuildingController_Functionality.md) #

# Flow Nodes #

## BuildingController:GeneralListener ##
Purpose: This node can be used to report back general events from a controller, including when the controller is validated/reset or when its power status changes.

Input Ports:
  * Enable - (Boolean) Set to TRUE to enable listening, FALSE to disable listening.
  * Team - (String) Name of controller's owning team (Used to build GUID)
  * Class - (String) Name of controller's building class (Used to build GUID)

Output Ports:
  * Validated - Triggered when controller has been validated
  * Reset - Triggered when controller has been reset
  * PowerChanged - (Boolean) Triggered when power changes for controller. Sends out TRUE if power is on, FALSE if power is off.
  * Destroyed - Triggered when controller is destroyed

## BuildingController:DamageListener ##
Purpose: This node can be used to report back damage-specific events from a controller.

Input Ports:
  * Enable - (Boolean) Set to TRUE to enable listening, FALSE to disable listening.
  * Team - (String) Name of controller's owning team (Used to build GUID)
  * Class - (String) Name of controller's building class (Used to build GUID)
  * Hits - (Boolean) TRUE to listen for weapon hits (non-explosions)
  * Explosions - (Boolean) TRUE to listen for explosions
  * ShooterId - (EntityId) When connected, limit HitInfo to this shooter
  * TargetId - (EntityId) When connected, limit HitInfo to this target
  * Weapon - (String) When set, limit HitInfo to this weapon
  * Ammo - (String) When set, limit HitInfo to this ammo

Output Ports:
  * OnDamage - Triggered when damage has occured
  * ShooterId - EntityID of the Shooter
  * TargetId - EntityID of the Target
  * WeaponId - EntityID of the Weapon
  * ProjectileId - EntityID of the Projectile if it was a bullet hit
  * HitPos - Position of the Hit
  * HitDir - Direction of the Hit
  * HitNormal - Normal of the Hit Impact
  * HitType - Name of the HitType
  * Damage - Damage amout which was caused
  * Material - Name of the Material

## BuildingController:ViewListener ##
Purpose: This node can be used to report back view-specific events from a controller.

Input Ports:
  * Enable - (Boolean) Set to TRUE to enable listening, FALSE to disable listening.
  * Team - (String) Name of controller's owning team (Used to build GUID)
  * Class - (String) Name of controller's building class (Used to build GUID)

Output Ports:
  * InView - Triggered when controller is in view by local client
  * OutOfView - Triggered when controller is no loner in view by local client

## BuildingController:HasPower ##
Purpose: This node can be used to check the power status of a building.

Input Ports:
  * Check - Call to check the power status of the controller.
  * Team - (String) Name of controller's owning team (Used to build GUID)
  * Class - (String) Name of controller's building class (Used to build GUID)

Output Ports:
  * Status - (Boolean) Triggered when status has been retrieved. Sends out TRUE if power is on, or FALSE if power is off.
  * Power - Triggered when status has been retrieved and power is on.
  * NoPower - Triggered when status has been retrieved and power is off.

## BuildingController:SetPower ##
Purpose: This node can be used to set the power status of a building.

Input Ports:
  * Set - (Boolean) Call to set the power status of the controller. Pass in TRUE to turn the power on, or FALSE to turn the power off.
  * SetWithValue - Call to set the power status of the controller using what is in the Value input port.
  * Team - (String) Name of controller's owning team (Used to build GUID)
  * Class - (String) Name of controller's building class (Used to build GUID)
  * Value - (Boolean) Power status to set when SetWithValue is called. TRUE to turn the power on, FALSE to turn the power off.

Output Ports:
  * Done - (Boolean) Triggered when power status has been set. Returns new power status of the building, TRUE if on or FALSE if off.

[Back](TechDoc_Architecture.md)