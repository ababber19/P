namespace Microsoft.Pc.TypeChecker
{
    public class SequenceType : PLanguageType
    {
        public SequenceType(PLanguageType elementType) : base(TypeKind.Sequence)
        {
            ElementType = elementType;
        }

        public PLanguageType ElementType { get; set; }

        public override string OriginalRepresentation => $"seq[{ElementType.OriginalRepresentation}]";
        public override string CanonicalRepresentation => $"seq[{ElementType.CanonicalRepresentation}]";

        public override bool IsAssignableFrom(PLanguageType otherType)
        {
            // Copying semantics: Can assign to a sequence variable if the other sequence's elements are subtypes of this sequence's elements.
            var other = otherType as SequenceType;
            return other != null && ElementType.IsAssignableFrom(other.ElementType);
        }

        public override PLanguageType Canonicalize() { return new SequenceType(ElementType.Canonicalize()); }
    }
}