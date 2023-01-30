-- save global functions
local require = require
-- setup the module namespace

-- print"已经不支持:"
print(module, table)

-- module "Event"
-- prepare auxiliary and prv tables
local aux, prv = {}, {}
-- local the C initialization function
--local initialize = require "C-Event"
-- obtain the private functions and provide access to auxiliary data 
-- and to the module namespace
local _M = {}
--initialize(aux, prv, _M)

print(_G)
print(_G["_VERSION"])
print(_VERSION)
print"hello main"

hello_c(0000)

function hello(...)
    print('in function hello:');
    print(select(1, ...))
    
    --error('hello err')
    return ...
end
