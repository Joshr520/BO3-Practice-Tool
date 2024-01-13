SetPlayerPoints(value)
{
    thread WriteToScreen("Setting Points To " + value);
    self.score = value;
}