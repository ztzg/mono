//
// TypeDefinition.cs
//
// Author:
//   Jb Evain (jbevain@gmail.com)
//
// (C) 2005 Jb Evain
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

namespace Mono.Cecil {

	public sealed class TypeDefinition : TypeReference, IMemberDefinition, IHasSecurity {

		TypeAttributes m_attributes;
		TypeReference m_baseType;

		bool m_hasInfo;
		ushort m_packingSize;
		uint m_classSize;

		InterfaceCollection m_interfaces;
		NestedTypeCollection m_nestedTypes;
		MethodDefinitionCollection m_methods;
		ConstructorCollection m_ctors;
		FieldDefinitionCollection m_fields;
		EventDefinitionCollection m_events;
		PropertyDefinitionCollection m_properties;
		SecurityDeclarationCollection m_secDecls;
		bool m_declaringTypeInitialized;		

		public override TypeReference DeclaringType {
			get {
				if (IsDelayedMode && !m_declaringTypeInitialized) {
					base.DeclaringType = MetaResolver.ResolveDeclaringType (MetadataToken);
					m_declaringTypeInitialized = true;
				}
				return base.DeclaringType;
			}
			set {
				m_declaringTypeInitialized = true;
				base.DeclaringType = value;
			}
		}

		public override CustomAttributeCollection CustomAttributes {
			get {
				if (m_customAttrs == null) {
					m_customAttrs = new CustomAttributeCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveCustomAttributes (MetadataToken, m_customAttrs);					
				}
				return m_customAttrs;
			}
		}

		public override GenericParameterCollection GenericParameters {
			get {
				if (m_genparams == null) {
					m_genparams = new GenericParameterCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveGenericParameters (MetadataToken, m_genparams);					
				}
				return m_genparams;
			}
		}

		public TypeAttributes Attributes {
			get { return m_attributes; }
			set { m_attributes = value; }
		}

		public TypeReference BaseType {
			get {
				if (m_baseType == null && IsDelayedMode)
					m_baseType = MetaResolver.ResolveBaseType (MetadataToken);
				return m_baseType;
			}
			set { m_baseType = value; }
		}

		public bool HasLayoutInfo {
			get { return m_hasInfo; }
		}

		public ushort PackingSize {
			get { return m_packingSize; }
			set {
				m_hasInfo = true;
				m_packingSize = value;
			}
		}

		public uint ClassSize {
			get { return m_classSize; }
			set {
				m_hasInfo = true;
				m_classSize = value;
			}
		}

		public InterfaceCollection Interfaces {
			get {
				if (m_interfaces == null) {
					m_interfaces = new InterfaceCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveInterfaces (MetadataToken, m_interfaces);											
				}

				return m_interfaces;
			}
		}

		public NestedTypeCollection NestedTypes {
			get {
				if (m_nestedTypes == null) {
					m_nestedTypes = new NestedTypeCollection (this);														
				}
				return m_nestedTypes;
			}
		}

		public MethodDefinitionCollection Methods {
			get {
				if (m_methods == null) {
					m_methods = new MethodDefinitionCollection (this);
					if (IsDelayedMode) {
						m_ctors = new ConstructorCollection (this);
						InitMethods ();
					}
				}

				return m_methods;
			}
		}

		public ConstructorCollection Constructors {
			get {
				if (m_ctors == null) {
					m_ctors = new ConstructorCollection (this);
					if (IsDelayedMode) {
						m_methods = new MethodDefinitionCollection (this);
						InitMethods ();
					}

				}

				return m_ctors;
			}
		}

		private void InitMethods ()
		{
			MetaResolver.ResolveMethods (MetadataToken, m_methods, m_ctors);			
		}

		public FieldDefinitionCollection Fields {
			get {
				if (m_fields == null) {
					m_fields = new FieldDefinitionCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveFields (MetadataToken, m_fields);						
				}

				return m_fields;
			}
		}

		public EventDefinitionCollection Events {
			get {
				if (m_events == null) {
					m_events = new EventDefinitionCollection (this);
					if (IsDelayedMode) {
						MetaResolver.ResolveEvents (MetadataToken, m_events);
					}
				}
				return m_events;
			}
		}

		public PropertyDefinitionCollection Properties {
			get {
				if (m_properties == null) {
					m_properties = new PropertyDefinitionCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveProperties (MetadataToken, m_properties);
				}
				return m_properties;
			}
		}

		public SecurityDeclarationCollection SecurityDeclarations {
			get {
				if (m_secDecls == null) {
					m_secDecls = new SecurityDeclarationCollection (this);
					if (IsDelayedMode)
						MetaResolver.ResolveSecurityDeclarations (MetadataToken, m_secDecls);					
				}
				return m_secDecls;
			}
		}

		#region TypeAttributes

		public bool IsNotPublic {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NotPublic; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NotPublic);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsPublic {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.Public; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.Public);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedPublic {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedPublic; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedPublic);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedPrivate {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedPrivate; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedPrivate);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedFamily {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedFamily; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedFamily);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedAssembly {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedAssembly; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedAssembly);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedFamilyAndAssembly {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedFamANDAssem; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedFamANDAssem);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsNestedFamilyOrAssembly {
			get { return (m_attributes & TypeAttributes.VisibilityMask) == TypeAttributes.NestedFamORAssem; }
			set {
				TypeAttributes masked = (TypeAttributes.VisibilityMask & TypeAttributes.NestedFamORAssem);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsAutoLayout {
			get { return (m_attributes & TypeAttributes.LayoutMask) == TypeAttributes.AutoLayout; }
			set {
				TypeAttributes masked = (TypeAttributes.LayoutMask & TypeAttributes.AutoLayout);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsSequentialLayout {
			get { return (m_attributes & TypeAttributes.LayoutMask) == TypeAttributes.SequentialLayout; }
			set {
				TypeAttributes masked = (TypeAttributes.LayoutMask & TypeAttributes.SequentialLayout);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsExplicitLayout {
			get { return (m_attributes & TypeAttributes.LayoutMask) == TypeAttributes.ExplicitLayout; }
			set {
				TypeAttributes masked = (TypeAttributes.LayoutMask & TypeAttributes.ExplicitLayout);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsClass {
			get { return (m_attributes & TypeAttributes.ClassSemanticMask) == TypeAttributes.Class; }
			set {
				TypeAttributes masked = (TypeAttributes.ClassSemanticMask & TypeAttributes.Class);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsInterface {
			get { return (m_attributes & TypeAttributes.ClassSemanticMask) == TypeAttributes.Interface; }
			set {
				TypeAttributes masked = (TypeAttributes.ClassSemanticMask & TypeAttributes.Interface);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsAbstract {
			get { return (m_attributes & TypeAttributes.Abstract) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.Abstract;
				else
					m_attributes &= ~TypeAttributes.Abstract;
			}
		}

		public bool IsSealed {
			get { return (m_attributes & TypeAttributes.Sealed) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.Sealed;
				else
					m_attributes &= ~TypeAttributes.Sealed;
			}
		}

		public bool IsSpecialName {
			get { return (m_attributes & TypeAttributes.SpecialName) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.SpecialName;
				else
					m_attributes &= ~TypeAttributes.SpecialName;
			}
		}

		public bool IsImport {
			get { return (m_attributes & TypeAttributes.Import) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.Import;
				else
					m_attributes &= ~TypeAttributes.Import;
			}
		}

		public bool IsSerializable {
			get { return (m_attributes & TypeAttributes.Serializable) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.Serializable;
				else
					m_attributes &= ~TypeAttributes.Serializable;
			}
		}

		public bool IsAnsiClass {
			get { return (m_attributes & TypeAttributes.StringFormatMask) == TypeAttributes.AnsiClass; }
			set {
				TypeAttributes masked = (TypeAttributes.StringFormatMask & TypeAttributes.AnsiClass);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsUnicodeClass {
			get { return (m_attributes & TypeAttributes.StringFormatMask) == TypeAttributes.UnicodeClass; }
			set {
				TypeAttributes masked = (TypeAttributes.StringFormatMask & TypeAttributes.UnicodeClass);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsAutoClass {
			get { return (m_attributes & TypeAttributes.StringFormatMask) == TypeAttributes.AutoClass; }
			set {
				TypeAttributes masked = (TypeAttributes.StringFormatMask & TypeAttributes.AutoClass);
				if (value)
					m_attributes |= masked;
				else
					m_attributes &= masked;
			}
		}

		public bool IsBeforeFieldInit {
			get { return (m_attributes & TypeAttributes.BeforeFieldInit) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.BeforeFieldInit;
				else
					m_attributes &= ~TypeAttributes.BeforeFieldInit;
			}
		}

		public bool IsRuntimeSpecialName {
			get { return (m_attributes & TypeAttributes.RTSpecialName) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.RTSpecialName;
				else
					m_attributes &= ~TypeAttributes.RTSpecialName;
			}
		}

		public bool HasSecurity {
			get { return (m_attributes & TypeAttributes.HasSecurity) != 0; }
			set {
				if (value)
					m_attributes |= TypeAttributes.HasSecurity;
				else
					m_attributes &= ~TypeAttributes.HasSecurity;
			}
		}

		#endregion

		public bool IsEnum {
			get { return BaseType != null && BaseType.FullName == Constants.Enum; }
		}

		public override bool IsValueType {
			get {
				return BaseType != null && (
					this.IsEnum || BaseType.FullName == Constants.ValueType);
			}
		}

		internal TypeDefinition (string name, string ns, TypeAttributes attrs) :
			base (name, ns)
		{
			m_hasInfo = false;
			m_attributes = attrs;
		}

		public TypeDefinition (string name, string ns,
			TypeAttributes attributes, TypeReference baseType) :
			this (name, ns, attributes)
		{
			this.BaseType = baseType;
		}

		public TypeDefinition Clone ()
		{
			return Clone (this, new ImportContext (NullReferenceImporter.Instance, this));
		}

		internal static TypeDefinition Clone (TypeDefinition type, ImportContext context)
		{
			TypeDefinition nt = new TypeDefinition (
				type.Name,
				type.Namespace,
				type.Attributes);

			context.GenericContext.Type = nt;

			foreach (GenericParameter p in type.GenericParameters)
				nt.GenericParameters.Add (GenericParameter.Clone (p, context));

			if (type.BaseType != null)
				nt.BaseType = context.Import (type.BaseType);

			if (type.HasLayoutInfo) {
				nt.ClassSize = type.ClassSize;
				nt.PackingSize = type.PackingSize;
			}

			foreach (FieldDefinition field in type.Fields)
				nt.Fields.Add (FieldDefinition.Clone (field, context));
			foreach (MethodDefinition ctor in type.Constructors)
				nt.Constructors.Add (MethodDefinition.Clone (ctor, context));
			foreach (MethodDefinition meth in type.Methods)
				nt.Methods.Add (MethodDefinition.Clone (meth, context));
			foreach (EventDefinition evt in type.Events)
				nt.Events.Add (EventDefinition.Clone (evt, context));
			foreach (PropertyDefinition prop in type.Properties)
				nt.Properties.Add (PropertyDefinition.Clone (prop, context));
			foreach (TypeReference intf in type.Interfaces)
				nt.Interfaces.Add (context.Import (intf));
			foreach (TypeDefinition nested in type.NestedTypes)
				nt.NestedTypes.Add (Clone (nested, context));
			foreach (CustomAttribute ca in type.CustomAttributes)
				nt.CustomAttributes.Add (CustomAttribute.Clone (ca, context));
			foreach (SecurityDeclaration dec in type.SecurityDeclarations)
				nt.SecurityDeclarations.Add (SecurityDeclaration.Clone (dec));

			return nt;
		}

		public override void Accept (IReflectionVisitor visitor)
		{
			visitor.VisitTypeDefinition (this);

			this.GenericParameters.Accept (visitor);
			this.Interfaces.Accept (visitor);
			this.Constructors.Accept (visitor);
			this.Methods.Accept (visitor);
			this.Fields.Accept (visitor);
			this.Properties.Accept (visitor);
			this.Events.Accept (visitor);
			this.NestedTypes.Accept (visitor);
			this.CustomAttributes.Accept (visitor);
			this.SecurityDeclarations.Accept (visitor);
		}
	}
}
