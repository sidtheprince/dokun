Item = {}
Item.__index=Item

function Item:new(name)
   local item = {}
   item.name = name

   setmetatable(item, Item)
   return item
end

function Item:get_name()
    return self.name
end

pot = Item:new("Pot")
print(pot:get_name())
print(getmetatable(pot) == Item)
