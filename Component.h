namespace adsk { namespace fusion {

/// Represents a component in the data model.
/// A component represents a set of geometry, features, and parameters that make up an item in the design.
/// A component can be referenced multiple times into a design with an Occurrence object.
class Component : public BaseComponent {
public:

   /// Returns the sketches collection associated with this component.
   /// This provides access to the existing sketches and supports
   /// the creation of new sketches.
   core::Ptr<Sketches> sketches() const;

   /// Returns the collection that provides access to all of the features
   /// associated with this component.
   core::Ptr<Features> features() const;

   /// Returns the mesh bodies collection associated with this component.
   core::Ptr<MeshBodies> meshBodies() const;

   /// Gets the transform matrix of the component.
   /// Returns a Matrix3D object representing the transform.
   core::Ptr<core::Matrix3D> transform() const;

   /// Sets the transform matrix of the component.
   /// transformMatrix : A Matrix3D object representing the new transform.
   /// Returns true if the transform was successfully set.
   bool transform(const core::Ptr<core::Matrix3D>& transformMatrix);

private:
   virtual core::Matrix3D* transform_raw() const = 0;
   virtual bool transform_raw(core::Matrix3D* transformMatrix) = 0;
};
