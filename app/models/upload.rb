class Upload < ApplicationRecord
        mount_uploader :uploaded_document, DocumentUploader
end
