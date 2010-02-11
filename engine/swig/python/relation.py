
%feature("shadow") SRelation::__str__()
%{
    def __str__(self):
        return self.to_string()
%}    

%feature("shadow") SRelation::to_string(const char *)
%{
    def to_string(self, prefix=""):
        stri = "%sRelation \'%s\':\n" %(prefix, self.name())
        item_prefix = "%s    " %prefix
        for i in self:
            stri += i.to_string(prefix=item_prefix)

        return stri
%}
