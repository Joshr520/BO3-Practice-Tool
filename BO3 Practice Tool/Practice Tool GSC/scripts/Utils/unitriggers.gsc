GetUntriggerByOrigin(origin)
{
    foreach (stub in level._unitriggers.trigger_stubs) {
        if (!IsDefined(stub.origin)) {
            continue;
        }
        if (IsArray(origin)) {
            foreach (org in origin) {
                if (org == stub.origin) {
                    return stub;
                }
            }
        }
        else if (origin == stub.origin) {
            return stub;
        }
    }

    foreach (stub in level._unitriggers.dynamic_stubs) {
         if (!IsDefined(stub.origin)) {
            continue;
        }
        if (IsArray(origin)) {
            foreach (org in origin) {
                if (org == stub.origin) {
                    return stub;
                }
            }
        }
        else if (origin == stub.origin) {
            return stub;
        }
    }

    foreach (zone in level.zones) {
         if (!IsDefined(stub.origin)) {
            continue;
        }
        foreach (stub in zone.unitrigger_stubs) {
            if (IsArray(origin)) {
                foreach (org in origin) {
                    if (org == stub.origin) {
                        return stub;
                    }
                }
            }
            else if (origin == stub.origin) {
                return stub;
            }
        }
    }

    return undefined;
}

AttemptGetUnitriggerByOrigin(origin, timeout = undefined)
{
    if (IsDefined(timeout)) {
        self endon("unitrigger_attempt_timeout");
        self util::delay_notify(timeout, "unitrigger_attempt_timeout");
    }
    trig = GetUntriggerByOrigin(origin);
    while (!IsDefined(trig)) {
        wait 0.05;
        trig = GetUntriggerByOrigin(origin);
    }
    return trig;
}

BasicTrigPlayerCheck()
{
    if(self zm_utility::in_revive_trigger()) {
        return false;
    }
    if(self.is_drinking > 0) {
        return false;
    }
    if(!zm_utility::is_player_valid(self)) {
        return false;
    }
    return true;
}