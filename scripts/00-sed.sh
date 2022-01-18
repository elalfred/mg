
echo "find . -name '*.h' -o -name '*.c' | xargs sed -i.bak -e 's/$1/$2/g'; find . -name '*.bak' -print | xargs rm"
#find . -name '*.h' -o -name '*.c' | xargs sed -i.bak -e 's/$1/$2/g'; find . -name '*.bak' -print | xargs rm

#find . -name '*.h' -o -name '*.c' | xargs sed -i.bak -e 's/$1/$2/g'; find . -name '*.bak' -print | xargs rm

# 2015

# eval $(bash 00-sed.sh Trfm3DTransformPoint TransformPointTrfm3D)
# eval $(bash 00-sed.sh Trfm3DTransformVector TransformVectorTrfm3D)
# eval $(bash 00-sed.sh Trfm3DSetRotVec SetRotVecTrfm3D)
# eval $(bash 00-sed.sh Trfm3DSetRotAxis SetRotAxisTrfm3D)
# eval $(bash 00-sed.sh Trfm3DAddRotAxis AddRotAxisTrfm3D)
# eval $(bash 00-sed.sh NodeCloneParent CloneParentNode)
# eval $(bash 00-sed.sh UpdateWCnode nodeUpdateWC)
# eval $(bash 00-sed.sh UpdateGSnode nodeUpdateGS)
# eval $(bash 00-sed.sh NodeAttach AttachNode)
# eval $(bash 00-sed.sh NodeDetach DetachNode)
# eval $(bash 00-sed.sh AllChildsNodeDetach DetachAllChildsNode)
# eval $(bash 00-sed.sh NodeDraw DrawNode)
# eval $(bash 00-sed.sh BSphereIntersectBSphere BSphereBSphereIntersect)


# hauek EZ ditut aldatu

# eval $(bash 00-sed.sh BBoxBBoxIntersect IntersectBBoxBBox)
# eval $(bash 00-sed.sh BBoxPlaneIntersect IntersectBBoxPlane)
# eval $(bash 00-sed.sh BSpherePlaneIntersect IntersectBSpherePlane)
# eval $(bash 00-sed.sh BSphereBBoxIntersect IntersectBSphereBBox)


# 2014

# bash 00-sed.sh TransformVectorTrfm3D Trfm3DTransformVector
# bash 00-sed.sh SetRotVecTrfm3D Trfm3DSetRotVec
# bash 00-sed.sh SetRotAxisTrfm3D Trfm3DSetRotAxis
# bash 00-sed.sh AddRotAxisTrfm3D Trfm3DAddRotAxis
# bash 00-sed.sh IntersectBBoxBBox BBoxBBoxIntersect
# bash 00-sed.sh IntersectBBoxPlane BBoxPlaneIntersect
# bash 00-sed.sh IntersectBSphereBSphere BSphereIntersectBSphere
# bash 00-sed.sh IntersectBSpherePlane BSpherePlaneIntersect
# bash 00-sed.sh IntersectBSphereBBox BSphereBBoxIntersect
# bash 00-sed.sh CloneNodeParent NodeCloneParent
# bash 00-sed.sh nodeUpdateWC UpdateWCnode
# bash 00-sed.sh nodeUpdateGS UpdateGSnode
# bash 00-sed.sh AttachNode NodeAttach
# bash 00-sed.sh DetachNode NodeDetach
# bash 00-sed.sh DetachAllChildsNode AllChildsNodeDetach
# bash 00-sed.sh DrawNode NodeDraw
